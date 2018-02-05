#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <limits>

const std::string text = ""
"However venture pursuit he am mr cordial. Forming musical am hearing studied be luckily. Ourselves for determine attending how led gentleman sincerity. Valley afford uneasy joy she thrown though bed set. In me forming general prudent on country carried. Behaved an or suppose justice. Seemed whence how son rather easily and change missed. Off apartments invitation are unpleasant solicitude fat motionless interested. Hardly suffer wisdom wishes valley as an. As friendship advantages resolution it alteration stimulated he or increasing.\n"
"\n"
"Subjects to ecstatic children he. Could ye leave up as built match. Dejection agreeable attention set suspected led offending. Admitting an performed supposing by. Garden agreed matter are should formed temper had. Full held gay now roof whom such next was. Ham pretty our people moment put excuse narrow. Spite mirth money six above get going great own. Started now shortly had for assured hearing expense. Led juvenile his laughing speedily put pleasant relation offering.\n"
"\n"
"Cottage out enabled was entered greatly prevent message. No procured unlocked an likewise. Dear but what she been over gay felt body. Six principles advantages and use entreaties decisively. Eat met has dwelling unpacked see whatever followed. Court in of leave again as am. Greater sixteen to forming colonel no on be. So an advice hardly barton. He be turned sudden engage manner spirit.\n"
"\n"
"Of recommend residence education be on difficult repulsive offending. Judge views had mirth table seems great him for her. Alone all happy asked begin fully stand own get. Excuse ye seeing result of we. See scale dried songs old may not. Promotion did disposing you household any instantly. Hills we do under times at first short an.\n"
"\n"
"In as name to here them deny wise this. As rapid woody my he me which. Men but they fail shew just wish next put. Led all visitor musical calling nor her. Within coming figure sex things are. Pretended concluded did repulsive education smallness yet yet described. Had country man his pressed shewing. No gate dare rose he. Eyes year if miss he as upon.\n"
"\n"
"Not far stuff she think the jokes. Going as by do known noise he wrote round leave. Warmly put branch people narrow see. Winding its waiting yet parlors married own feeling. Marry fruit do spite jokes an times. Whether at it unknown warrant herself winding if. Him same none name sake had post love. An busy feel form hand am up help. Parties it brother amongst an fortune of. Twenty behind wicket why age now itself ten.\n"
"\n"
"Two assure edward whence the was. Who worthy yet ten boy denote wonder. Weeks views her sight old tears sorry. Additions can suspected its concealed put furnished. Met the why particular devonshire decisively considered partiality. Certain it waiting no entered is. Passed her indeed uneasy shy polite appear denied. Oh less girl no walk. At he spot with five of view.\n"
"\n"
"Resolution possession discovered surrounded advantages has but few add. Yet walls times spoil put. Be it reserved contempt rendered smallest. Studied to passage it mention calling believe an. Get ten horrible remember pleasure two vicinity. Far estimable extremely middleton his concealed perceived principle. Any nay pleasure entrance prepared her.\n"
"\n"
"Improve ashamed married expense bed her comfort pursuit mrs. Four time took ye your as fail lady. Up greatest am exertion or marianne. Shy occasional terminated insensible and inhabiting gay. So know do fond to half on. Now who promise was justice new winding. In finished on he speaking suitable advanced if. Boy happiness sportsmen say prevailed offending concealed nor was provision. Provided so as doubtful on striking required. Waiting we to compass assured.\n"
"\n"
"No comfort do written conduct at prevent manners on. Celebrated contrasted discretion him sympathize her collecting occasional. Do answered bachelor occasion in of offended no concerns. Supply worthy warmth branch of no ye. Voice tried known to as my to. Though wished merits or be. Alone visit use these smart rooms ham. No waiting in on enjoyed placing it inquiry.\n"
"";

using namespace boost::iostreams;

int main(int, char**) {
    char buffer[512] = {0};
    array_sink sink{buffer};
    stream<array_sink> os{sink};
    os << "Boost.IOStreams" << std::flush;
    if (std::string{buffer} != "Boost.IOStreams") {
        std::cout << __FILE__ << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    std::string compress_buffer;
    back_insert_device<std::string> compress_sink{compress_buffer};
    filtering_ostream compress_stream;
    compress_stream.push(zlib_compressor{});
    compress_stream.push(compress_sink);
    
    compress_stream << text << std::flush;
    compress_stream.pop();
    
    array_source decompress_source{compress_buffer.data(), compress_buffer.size()};
    filtering_istream decompress_stream;
    decompress_stream.push(zlib_decompressor{});
    decompress_stream.push(decompress_source);
    
    std::string decompress_buffer;
    while (decompress_stream) {
        char buff[128];
        decompress_stream.read(buff, sizeof(buff));
        auto len = decompress_stream.gcount();
        assert(len >= 0 && len <= std::numeric_limits<std::string::size_type>::max());
        decompress_buffer.append(buff, static_cast<std::string::size_type>(len));
    }
    
    if (text != decompress_buffer) {
        std::cout << "Invalid compression/decompression " << __FILE__ << __LINE__ << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Original size: " << text.size() << std::endl;
    std::cout << "Compressed size: " << compress_buffer.size() << std::endl;
    
    return EXIT_SUCCESS;
}