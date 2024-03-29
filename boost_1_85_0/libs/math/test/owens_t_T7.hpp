// Copyright (C) Benjamin Sobotta 2012

//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_OWENS_T7_HPP
#define BOOST_OWENS_T7_HPP

// Reference:
// Mike Patefield, David Tandy
// FAST AND ACCURATE CALCULATION OF OWEN'S T-FUNCTION
// Journal of Statistical Software, 5 (5), 1-25

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/math/special_functions/owens_t.hpp>
#include <vector>
#include <algorithm> // std::sort
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>

namespace boost
{
  namespace math
  {
    namespace detail
    {
      template<typename ndx_type, typename data_type>
      inline bool owens_t_sort_proxy(const ndx_type n1, const ndx_type n2, const data_type* data) { return (fabs(data[n1]) < fabs(data[n2])); }

      // compute the value of Owen's T function with method T7 from the reference paper
      template<typename RealType>
      inline RealType compute_owens_t_T7(const RealType h, const RealType a)
      {
        BOOST_MATH_STD_USING
        using namespace boost::math::constants;

        std::vector<RealType> c2(51);

        c2[0]=static_cast<RealType>(0.99999999999999999999999999999999999999868061520482714021759648020474317574987355923862368638038320094484952302876219e0l);
        c2[1]=static_cast<RealType>(-0.99999999999999999999999999999999999302462011069054837422425677393793571720981603447930035285521866357878934009159708e0l);
        c2[2]=static_cast<RealType>(0.9999999999999999999999999999999938559078263892797131217517182153540396811776160269133372859096210993958152102616265e0l);
        c2[3]=static_cast<RealType>(-0.99999999999999999999999999999783757316880746021776281028307338046033395960167168255107746699076325159707117075122598e0l);
        c2[4]=static_cast<RealType>(0.99999999999999999999999999959301304409247897363805160078169883190960201161021089063082244377785297302604378738778956e0l);
        c2[5]=static_cast<RealType>(-0.99999999999999999999999995245835998362821388118964869664453661644738440917895538883200648244745024569685062708634168e0l);
        c2[6]=static_cast<RealType>(0.99999999999999999999999622575084004275114986560329977419595335828482080658911071010175083376987487699799620433312026e0l);
        c2[7]=static_cast<RealType>(-0.9999999999999999999997835456196743116277756586095640965807421017047019205312795102010127835367920317933410096951991e0l);
        c2[8]=static_cast<RealType>(0.99999999999999999999063044410000386518700820850762955399774821040632831953277713720592510531609301137092442411088318e0l);
        c2[9]=static_cast<RealType>(-0.99999999999999999968363056215229124110523314446131391646528327924749429301803531868618645140379927991069880419895362e0l);
        c2[10]=static_cast<RealType>(0.99999999999999999145103710789254646427253519282003979608764987525701449150796674610676803428471393828460151017134006e0l);
        c2[11]=static_cast<RealType>(-0.99999999999999981131055812853212338067344201452850197384362641710824830755936071166619436310647545618172487913130615e0l);
        c2[12]=static_cast<RealType>(0.99999999999999654123165670138354353746969688832625358460636033129592270928035522003120035894014007078745281902561497e0l);
        c2[13]=static_cast<RealType>(-0.99999999999994661543772980917477218111196118287898714561621502747335071313305657131504975341280407503235841894095032e0l);
        c2[14]=static_cast<RealType>(0.9999999999992981439043273655170176041716503383856785977723125483146102600097712876017125231262708400720094556926045e0l);
        c2[15]=static_cast<RealType>(-0.99999999999206265150568436336459788968621075287629010127877385312281458164262755824589807456902954264068398140350718e0l);
        c2[16]=static_cast<RealType>(0.99999999992213502163371840879683818667748180414402080554120502891144910024410028330131981216472217247119418222397804e0l);
        c2[17]=static_cast<RealType>(-0.99999999933259095924055744571071653706200038213621273711937864999491782183629569409353668088363952580491517608609879e0l);
        c2[18]=static_cast<RealType>(0.99999999497015572069563854257687102391713999264054303202862355260549032899913629304955062400529548430606517184769124e0l);
        c2[19]=static_cast<RealType>(-0.99999996648712934179852410529668328351470567694495362454934194838547830786603251986748376180326858512474939517328148e0l);
        c2[20]=static_cast<RealType>(0.9999998016410566689368417618988041312715506400990955108339485167214078601378203496694653725800913994771609966243554e0l);
        c2[21]=static_cast<RealType>(-0.9999989525985184659277351038817797931250021614576921513415321341983217728506335369734522548408960126473437428356652e0l);
        c2[22]=static_cast<RealType>(0.99999504755520008311742537842275606593998257673570772153723704887194777830960070883905373411740901379060307967971713e0l);
        c2[23]=static_cast<RealType>(-0.99997896168242457050517756812692812892733179258193505494454091938132703728342085231042113297420877668525201728601112e0l);
        c2[24]=static_cast<RealType>(0.99991946779137118276149990348538316553731391341330104178195711109963439425910633064752492021322249165299888263409172e0l);
        c2[25]=static_cast<RealType>(-0.99972148778885346568502104725962502476360296866194523161605564314646459144266349095901801601276421334075117884929242e0l);
        c2[26]=static_cast<RealType>(0.9991276859682010382270359712512423427555252098639083241851998143073842159537070734558612169871757287024196969316524e0l);
        c2[27]=static_cast<RealType>(-0.99752026434927262148166061506294447561314364149201350129770769406693673101306930831078789512571673723645189614485884e0l);
        c2[28]=static_cast<RealType>(0.99358896672458664010385299342636899693001298318272470058323214841807825867323354755879957618925036849552478542814464e0l);
        c2[29]=static_cast<RealType>(-0.98489603971030687415560911591294255813647866616337974199082847337212210737029861747833396359832470961032052017929136e0l);
        c2[30]=static_cast<RealType>(0.96751068500593933205520070783164495637686016604265460899685963694355146172759956614360799629964537023691986327884468e0l);
        c2[31]=static_cast<RealType>(-0.9360616426988023756922055734494146109996789121506654176278348120618330748262465556674776006234661241542758473118521e0l);
        c2[32]=static_cast<RealType>(0.88462086639047762401913298378165508003735156726358522187746603832270096575743334309836954108840694198036767569315108e0l);
        c2[33]=static_cast<RealType>(-0.80858877429268840241019894355701184332631739713578890873529014128055757110890871538802016131209054476205332440695604e0l);
        c2[34]=static_cast<RealType>(0.70714608761601125771088731886413274896589865282669140675007048926200481310909899767964678381587391145800655465120571e0l);
        c2[35]=static_cast<RealType>(-0.58515160223005410186594841297741982997622522778974325847124400853371098492516978008742019478185592841561858134639963e0l);
        c2[36]=static_cast<RealType>(0.45317050454923571830339270216379220523529541036307428556285489535205431482111178945999365703295821120700802922701552e0l);
        c2[37]=static_cast<RealType>(-0.32503812067644347537685664861030147676006324710175782617307839344555807653091746007670995202936607469695700327677892e0l);
        c2[38]=static_cast<RealType>(0.2137534704367539657335116107238861128574908030994099713290249568609752278177667873985860406515292692728315916731017e0l);
        c2[39]=static_cast<RealType>(-0.127619403804624293099162792279930050338998502678126917236608595894255012173888208013041488822409228894596510380150144e0l);
        c2[40]=static_cast<RealType>(0.06848690630890464573940500271274680442209417344598560366267896451733825978600890476338760194302261371849816636335383e0l);
        c2[41]=static_cast<RealType>(-0.032688749970534932162217917343517016881654494605298904939793624525513795530795862058337162798487051632431756396211348e0l);
        c2[42]=static_cast<RealType>(0.013715287119044125499391779954972556538977518079926978123084819229658358422347583377635796490450010646568919625741353e0l);
        c2[43]=static_cast<RealType>(-0.0049902360281804772302071182896105260677340309700627303663654639589340705520269439056577683691783146342849984775533766e0l);
        c2[44]=static_cast<RealType>(0.00154866713536508358511616306566119586713663676744267610952381384231395851333350529444375690335427250570956017759427897e0l);
        c2[45]=static_cast<RealType>(-4.0136222792839484462604207322390667014747359362667770129433998717936089480817561247228048128811298286479443203785308e-4l);
        c2[46]=static_cast<RealType>(8.4429279202769263712337394035686562275089641577318336414305767736129198590671750045859928722396139236629554169757073e-5l);
        c2[47]=static_cast<RealType>(-1.3837048406451028003397758396690378745165135180030260294919960014069368993089617158234925766366971794969129152002478e-5l);
        c2[48]=static_cast<RealType>(1.6563766752141156889761471523635738102614710503912266484040958175597362783384628269667016371235475899745552923626272e-6l);
        c2[49]=static_cast<RealType>(-1.2874491982958001084373112367555214480376326465550959775329113928120230317885310174458065240563391789639012991976545e-7l);
        c2[50]=static_cast<RealType>(4.8740773419420086658863239448043342637632445767226310886251599519774455542302728072876112682268516221911975181091336e-9l);

        const RealType hs = h*h;
        const RealType as = a*a;

        //RealType u = one_div_two_pi<RealType>() * a * exp(-half<RealType>()*hs*(static_cast<RealType>(1)+as));
        RealType v = c2[0];

        RealType val = v;
        RealType last_val = val+1; // last_val must not be the same as val

        int k = 0;

        std::vector<RealType> memory;
        memory.push_back(v);

        while((val != last_val) || (k<51)) // use all c2
        {
          last_val = val;
          k++;
          const RealType u = pow(as,k);
          if(k < static_cast<int>(c2.size()))
          {
            v = (hs*v + c2[k])/(static_cast<RealType>(2*k+1));
          }
          else
          {
            // assume that for numerical purposes c2[k]==0
            v = hs*v/(static_cast<RealType>(2*k+1));
          }
          val += u*v;
          if(val >= tools::max_value<RealType>())
            break;
          memory.push_back(u*v);
        } // while(val != last_val)

        std::vector<unsigned> ndx_4_sorted_data(memory.size());
        for(unsigned i = 0; i != memory.size(); ++i)
        {
          ndx_4_sorted_data[i]=i;
        }

        std::sort(ndx_4_sorted_data.begin(), ndx_4_sorted_data.end(),
                  boost::bind(owens_t_sort_proxy<typename std::vector<RealType>::size_type, RealType>, _1, _2, &memory[0]));

        val = memory[ndx_4_sorted_data[0]];
        for(unsigned i = 1; i != memory.size(); i++)
        {
          val+=memory[ndx_4_sorted_data[i]];
        }

        // split the exponential to avoid values that go below the minimum floating pt value
        val *= exp(-half<RealType>()*hs*as);
        val *= exp(-half<RealType>()*hs);
        val *= one_div_two_pi<RealType>() * a;

        return val;
      } // RealType compute_owens_t_T7(const RealType h, const RealType a)

      // compute Owen's T function, T(h,a), for arbitrary values of h and a
     template<typename RealType, class Policy>
     inline RealType owens_t_T7(RealType h, RealType a, const Policy& pol)
     {
        BOOST_MATH_STD_USING
        // exploit that T(-h,a) == T(h,a)
        h = fabs(h);

        // Use equation (2) in the paper to remap the arguments
        // such that h>=0 and 0<=a<=1 for the call of the actual
        // computation routine.

        const RealType fabs_a = fabs(a);
        const RealType fabs_ah = fabs_a*h;

        RealType val = 0.0; // avoid compiler warnings, 0.0 will be overwritten in any case

        if(fabs_a <= 1)
        {
           val = compute_owens_t_T7(h, fabs_a);
        } // if(fabs_a <= 1.0)
        else
        {
           if( h <= 0.67 )
           {
              const RealType normh = owens_t_znorm1(h, pol);
              const RealType normah = owens_t_znorm1(fabs_ah, pol);
              val = static_cast<RealType>(1)/static_cast<RealType>(4) - normh*normah -
                 compute_owens_t_T7(fabs_ah, static_cast<RealType>(1 / fabs_a));
           } // if( h <= 0.67 )
           else
           {
              const RealType normh = owens_t_znorm2(h, pol);
              const RealType normah = owens_t_znorm2(fabs_ah, pol);
              val = constants::half<RealType>()*(normh+normah) - normh*normah -
                 compute_owens_t_T7(fabs_ah, static_cast<RealType>(1 / fabs_a));
           } // else [if( h <= 0.67 )]
        } // else [if(fabs_a <= 1)]

        // exploit that T(h,-a) == -T(h,a)
        if(a < 0)
        {
           return -val;
        } // if(a < 0)

        return val;
      } // RealType owens_t(RealType h, RealType a)

    } // namespace detail

    template <class T1, class T2, class Policy>
    inline typename tools::promote_args<T1, T2>::type owens_t_T7(T1 h, T2 a, const Policy& pol)
    {
       typedef typename tools::promote_args<T1, T2>::type result_type;
       typedef typename policies::evaluation<result_type, Policy>::type value_type;
       return policies::checked_narrowing_cast<result_type, Policy>(detail::owens_t_T7(static_cast<value_type>(h), static_cast<value_type>(a), pol), "boost::math::owens_t_T7<%1%>(%1%,%1%)");
    }

    template <class T1, class T2>
    inline typename tools::promote_args<T1, T2>::type owens_t_T7(T1 h, T2 a)
    {
       return owens_t_T7(h, a, policies::policy<>());
    }

  } // namespace math
} // namespace boost

#endif





