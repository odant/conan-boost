/*
 * Copyright 1993, 2000 Christopher Seiwald.
 *
 * This file is part of Jam - see jam.c for Copyright information.
 */

/*  This file is ALSO:
 *  Copyright 2022 René Ferdinand Rivera Morell
 *  Copyright 2001-2004 David Abrahams.
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
 */

#include "jam.h"
#include "lists.h"
#include "parse.h"
#include "scan.h"
#include "object.h"
#include "modules.h"
#include "frames.h"
#include "function.h"
#include "mem.h"
#include "startup.h"
#include "rules.h"
#include "search.h"

#include <cstring>
#include <set>
#include <string>
#include <vector>

/*
 * parse.c - make and destroy parse trees as driven by the parser
 *
 * 09/07/00 (seiwald) - ref count on PARSE to avoid freeing when used,
 *      as per Matt Armstrong.
 * 09/11/00 (seiwald) - structure reworked to reflect that (*func)()
 *      returns a LIST *.
 */

static PARSE * yypsave;
static std::set<PARSE*> parse_mem;


struct parse_ptr
{
    parse_ptr() : ptr( yypsave ) {}
    ~parse_ptr() { if ( ptr ) parse_free( ptr ); }
    operator PARSE*() const { return ptr; }

    private:
    PARSE * ptr = nullptr;
};

static void parse_impl( FRAME * frame )
{

    /* Now parse each block of rules and execute it. Execute it outside of the
     * parser so that recursive calls to yyrun() work (no recursive yyparse's).
     */

    for ( ; ; )
    {
        /* Filled by yyparse() calling parse_save(). */
        yypsave = nullptr;

        /* If parse error or empty parse, outta here. */
        int yy_result = yyparse();
        parse_ptr p;
        if ( yy_result || !p )
            break;

        /* Compile the parse tree. */
        auto func = b2::jam::make_unique_bare_jptr( function_compile( p ), function_free );

        /* Run the parsed function. */
        list_free( function_run( func.get(), frame ) );
    }

    yyfdone();
}


void parse_file( OBJECT * f, FRAME * frame )
{
    /* Suspend scan of current file and push this new file in the stream. */
    yyfparse( f );

    parse_impl( frame );
}


void parse_string( OBJECT * name, const char * * lines, FRAME * frame )
{
    yysparse( name, lines );
    parse_impl( frame );
}


void parse_buffer( OBJECT * name, const char * buffer, FRAME * frame )
{
    std::size_t buffer_s = std::strlen(buffer);
    std::string strings;
    strings.reserve(buffer_s*2);
    std::vector<const char *> lines;
    const char * buffer_e = buffer+buffer_s;
    for (const char * buffer_i = buffer; buffer_i && buffer_i != buffer_e; )
    {
        const char * eol = std::strchr(buffer_i, '\n');
        if (eol)
        {
            strings.append(buffer_i, eol+1);
            strings.append(1, '\0');
            buffer_i = eol+1;
        }
        else
        {
            strings.append(buffer_i);
            buffer_i = nullptr;
        }
    }
    for (const char * strings_i = strings.c_str();
        strings_i != strings.c_str()+strings.size(); )
    {
        lines.push_back(strings_i);
        strings_i += std::strlen(strings_i)+1;
    }
    lines.push_back(nullptr);
    parse_string( name, lines.data(), frame );
}


void parse_include( OBJECT * target, FRAME * frame )
{
    TARGET * const t = bindtarget( target );

    /* DWA 2001/10/22 - Perforce Jam cleared the arguments here,
        * which prevented an included file from being treated as part
        * of the body of a rule. I did not see any reason to do that,
        * so I lifted the restriction.
        */

    /* Bind the include file under the influence of "on-target"
        * variables. Though they are targets, include files are not
        * built with make().
        */

    pushsettings( root_module(), t->settings );
    /* We do not expect that a file to be included is generated by
        * some action. Therefore, pass 0 as third argument. If the name
        * resolves to a directory, let it error out.
        */
    object_free( t->boundname );
    t->boundname = search( t->name, &t->time, 0, 0 );
    popsettings( root_module(), t->settings );

    parse_file( t->boundname, frame );
}


void parse_save( PARSE * p )
{
    yypsave = p;
}


PARSE * parse_make(
    int      type,
    PARSE *& left,
    PARSE *& right,
    PARSE *& third,
    OBJECT * string,
    OBJECT * string1,
    int      num )
{
    PARSE * p = (PARSE *)BJAM_MALLOC( sizeof( PARSE ) );

    p->type = type;
    p->left = left; left = nullptr;
    p->right = right; right = nullptr;
    p->third = third; third = nullptr;
    p->string = string;
    p->string1 = string1;
    p->num = num;
    p->rulename = 0;

    if ( p->left )
    {
        p->file = object_copy( p->left->file );
        p->line = p->left->line;
    }
    else
    {
        yyinput_last_read_token( &p->file, &p->line );
        p->file = object_copy( p->file );
    }

    parse_mem.insert( p );

    return p;
}


void parse_free( PARSE * & p )
{
    if ( parse_mem.count( p ) == 0 ) return;
    parse_mem.erase( p );

    if ( p->string )
        object_free( p->string );
    if ( p->string1 )
        object_free( p->string1 );
    if ( p->left )
        parse_free( p->left );
    if ( p->right )
        parse_free( p->right );
    if ( p->third )
        parse_free( p->third );
    if ( p->rulename )
        object_free( p->rulename );
    if ( p->file )
        object_free( p->file );

    BJAM_FREE( (char *)p );
    p = nullptr;
}


void parse_done()
{
    while ( parse_mem.size() > 0 )
    {
        PARSE * p = *parse_mem.begin();
        parse_free( p );
    }
}
