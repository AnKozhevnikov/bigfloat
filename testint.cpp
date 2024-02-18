#include "bigint.h"
#include <catch2/catch_test_macros.hpp>

using namespace aak;

bigint nan_sample("aaa");
bigint undef_sample=0_bi/0_bi;
bigint inf_sample=1_bi/0_bi;
bigint int_sample=10_bi;
bigint null_sample=0_bi;

TEST_CASE("basic operations") {
    SECTION("addition") {
        REQUIRE(0_bi+0_bi == 0_bi);
        REQUIRE(0_bi+1_bi == 1_bi);
        REQUIRE(1_bi+0_bi == 1_bi);
        REQUIRE(1_bi+1_bi == 2_bi);
        REQUIRE(-1_bi+0_bi == -1_bi);
        REQUIRE(0_bi+-1_bi == -1_bi);
        REQUIRE(-1_bi+-1_bi == -2_bi);
        REQUIRE(1_bi+-1_bi == 0_bi);
        REQUIRE(100_bi+100_bi == 200_bi);
        REQUIRE(100_bi+10_bi == 110_bi);
        REQUIRE(10000000000000000000000000000000000000000000000_bi+10000000000000000000000000000000000000000000000_bi == 20000000000000000000000000000000000000000000000_bi);
        REQUIRE(-10000000000000000000000000000000000000000000000_bi+10000000000000000000000000000000000000000000000_bi == 0_bi);
        REQUIRE(123_bi+456_bi == 579_bi);
        REQUIRE(32_bi+32_bi == 64_bi);
        REQUIRE(4294967296_bi+4294967296_bi == 8589934592_bi);
        REQUIRE(18446744073709551616_bi+18446744073709551616_bi == 36893488147419103232_bi);

        bigint a=0;
        for (int i=0; i<1000; i++) a+=1;
        REQUIRE(a==1000_bi);

        REQUIRE((nan_sample+nan_sample).is_nan());
        REQUIRE((nan_sample+undef_sample).is_nan());
        REQUIRE((nan_sample+inf_sample).is_nan());
        REQUIRE((nan_sample+int_sample).is_nan());
        REQUIRE((nan_sample+null_sample).is_nan());

        REQUIRE((undef_sample+nan_sample).is_nan());
        REQUIRE((undef_sample+undef_sample).is_undef());
        REQUIRE((undef_sample+inf_sample).is_undef());
        REQUIRE((undef_sample+int_sample).is_undef());
        REQUIRE((undef_sample+null_sample).is_undef());

        REQUIRE((inf_sample+nan_sample).is_nan());
        REQUIRE((inf_sample+undef_sample).is_undef());
        REQUIRE((inf_sample+inf_sample).is_undef());
        REQUIRE((inf_sample+int_sample).is_infinity());
        REQUIRE((inf_sample+null_sample).is_infinity());

        REQUIRE((int_sample+nan_sample).is_nan());
        REQUIRE((int_sample+undef_sample).is_undef());
        REQUIRE((int_sample+inf_sample).is_infinity());
        REQUIRE((int_sample+int_sample).is_normal());
        REQUIRE((int_sample+null_sample).is_normal());

        REQUIRE((null_sample+nan_sample).is_nan());
        REQUIRE((null_sample+undef_sample).is_undef());
        REQUIRE((null_sample+inf_sample).is_infinity());
        REQUIRE((null_sample+int_sample).is_normal());
        REQUIRE((null_sample + null_sample).is_zero());
    }
    SECTION("subtraction") {
        REQUIRE(0_bi-0_bi==0_bi);
        REQUIRE(1_bi-1_bi==0_bi);
        REQUIRE(1000000000000000000000000_bi-1000000000000000000000000_bi==0_bi);
        REQUIRE(123_bi-456_bi==-333_bi);
        REQUIRE(-18446744073709551616_bi-18446744073709551616_bi == -36893488147419103232_bi);

        REQUIRE((nan_sample-nan_sample).is_nan());
        REQUIRE((nan_sample-undef_sample).is_nan());
        REQUIRE((nan_sample-inf_sample).is_nan());
        REQUIRE((nan_sample-int_sample).is_nan());
        REQUIRE((nan_sample-null_sample).is_nan());

        REQUIRE((undef_sample-nan_sample).is_nan());
        REQUIRE((undef_sample-undef_sample).is_undef());
        REQUIRE((undef_sample-inf_sample).is_undef());
        REQUIRE((undef_sample-int_sample).is_undef());
        REQUIRE((undef_sample-null_sample).is_undef());

        REQUIRE((inf_sample-nan_sample).is_nan());
        REQUIRE((inf_sample-undef_sample).is_undef());
        REQUIRE((inf_sample-inf_sample).is_undef());
        REQUIRE((inf_sample-int_sample).is_infinity());
        REQUIRE((inf_sample-null_sample).is_infinity());

        REQUIRE((int_sample-nan_sample).is_nan());
        REQUIRE((int_sample-undef_sample).is_undef());
        REQUIRE((int_sample-inf_sample).is_infinity());
        REQUIRE((int_sample-int_sample).is_normal());
        REQUIRE((int_sample-null_sample).is_normal());

        REQUIRE((null_sample-nan_sample).is_nan());
        REQUIRE((null_sample-undef_sample).is_undef());
        REQUIRE((null_sample-inf_sample).is_infinity());
        REQUIRE((null_sample-int_sample).is_normal());
        REQUIRE((null_sample - null_sample).is_zero());
    }
    SECTION("multiplication") {
        REQUIRE(0_bi*0_bi==0_bi);
        REQUIRE(1_bi*0_bi==0_bi);
        REQUIRE(0_bi*1_bi==0_bi);
        REQUIRE(32_bi*32_bi==1024_bi);
        REQUIRE(-32_bi*32_bi==-1024_bi);
        REQUIRE(123_bi*456_bi==56088_bi);

        bigint a=1;
        for (int i=0; i<64; i++) a*=2;
        REQUIRE(a==18446744073709551616_bi);

        REQUIRE((nan_sample*nan_sample).is_nan());
        REQUIRE((nan_sample*undef_sample).is_nan());
        REQUIRE((nan_sample*inf_sample).is_nan());
        REQUIRE((nan_sample*int_sample).is_nan());
        REQUIRE((nan_sample*null_sample).is_nan());

        REQUIRE((undef_sample*nan_sample).is_nan());
        REQUIRE((undef_sample*undef_sample).is_undef());
        REQUIRE((undef_sample*inf_sample).is_undef());
        REQUIRE((undef_sample*int_sample).is_undef());
        REQUIRE((undef_sample*null_sample).is_undef());

        REQUIRE((inf_sample*nan_sample).is_nan());
        REQUIRE((inf_sample*undef_sample).is_undef());
        REQUIRE((inf_sample*inf_sample).is_infinity());
        REQUIRE((inf_sample*int_sample).is_infinity());
        REQUIRE((inf_sample*null_sample).is_undef());

        REQUIRE((int_sample*nan_sample).is_nan());
        REQUIRE((int_sample*undef_sample).is_undef());
        REQUIRE((int_sample*inf_sample).is_infinity());
        REQUIRE((int_sample*int_sample).is_normal());
        REQUIRE((int_sample * null_sample).is_zero());

        REQUIRE((null_sample*nan_sample).is_nan());
        REQUIRE((null_sample*undef_sample).is_undef());
        REQUIRE((null_sample*inf_sample).is_undef());
        REQUIRE((null_sample * int_sample).is_zero());
        REQUIRE((null_sample * null_sample).is_zero());
    }
    SECTION("division") {
        REQUIRE(0_bi/1_bi==0_bi);
        REQUIRE(-5342986785638439792634932673645938258263675784_bi/1_bi==-5342986785638439792634932673645938258263675784_bi);
        REQUIRE(1024_bi/2_bi==512_bi);
        REQUIRE(36893488147419103232_bi/2_bi==18446744073709551616_bi);
        REQUIRE(3_bi/2_bi==1_bi);
        REQUIRE(2_bi/3_bi==0_bi);
        REQUIRE(100_bi/10_bi==10_bi);
        REQUIRE(99_bi/10_bi==9_bi);

        REQUIRE((nan_sample/nan_sample).is_nan());
        REQUIRE((nan_sample/undef_sample).is_nan());
        REQUIRE((nan_sample/inf_sample).is_nan());
        REQUIRE((nan_sample/int_sample).is_nan());
        REQUIRE((nan_sample/null_sample).is_nan());

        REQUIRE((undef_sample/nan_sample).is_nan());
        REQUIRE((undef_sample/undef_sample).is_undef());
        REQUIRE((undef_sample/inf_sample).is_undef());
        REQUIRE((undef_sample/int_sample).is_undef());
        REQUIRE((undef_sample/null_sample).is_undef());

        REQUIRE((inf_sample/nan_sample).is_nan());
        REQUIRE((inf_sample/undef_sample).is_undef());
        REQUIRE((inf_sample/inf_sample).is_undef());
        REQUIRE((inf_sample/int_sample).is_infinity());
        REQUIRE((inf_sample/null_sample).is_infinity());

        REQUIRE((int_sample/nan_sample).is_nan());
        REQUIRE((int_sample/undef_sample).is_undef());
        REQUIRE((int_sample / inf_sample).is_zero());
        REQUIRE((int_sample/int_sample).is_normal());
        REQUIRE((int_sample/null_sample).is_infinity());

        REQUIRE((null_sample/nan_sample).is_nan());
        REQUIRE((null_sample/undef_sample).is_undef());
        REQUIRE((null_sample / inf_sample).is_zero());
        REQUIRE((null_sample / int_sample).is_zero());
        REQUIRE((null_sample/null_sample).is_undef());
    }
    SECTION("modulo") {
        REQUIRE(0_bi%10_bi==0_bi);
        REQUIRE(100_bi%3_bi==1_bi);
        REQUIRE(-100_bi%3_bi==-1_bi);
        REQUIRE(-100_bi%-3_bi==-1_bi);
        REQUIRE(100_bi%-3_bi==1_bi);
        REQUIRE(534825983275683295874395823759234_bi%10==4_bi);
        REQUIRE(5328795634_bi%9817428196452897346289743264_bi==5328795634_bi);

        REQUIRE((nan_sample%nan_sample).is_nan());
        REQUIRE((nan_sample%undef_sample).is_nan());
        REQUIRE((nan_sample%inf_sample).is_nan());
        REQUIRE((nan_sample%int_sample).is_nan());
        REQUIRE((nan_sample%null_sample).is_nan());

        REQUIRE((undef_sample%nan_sample).is_nan());
        REQUIRE((undef_sample%undef_sample).is_undef());
        REQUIRE((undef_sample%inf_sample).is_undef());
        REQUIRE((undef_sample%int_sample).is_undef());
        REQUIRE((undef_sample%null_sample).is_undef());

        REQUIRE((inf_sample%nan_sample).is_nan());
        REQUIRE((inf_sample%undef_sample).is_undef());
        REQUIRE((inf_sample%inf_sample).is_undef());
        REQUIRE((inf_sample%int_sample).is_undef());
        REQUIRE((inf_sample%null_sample).is_undef());

        REQUIRE((int_sample%nan_sample).is_nan());
        REQUIRE((int_sample%undef_sample).is_undef());
        REQUIRE((int_sample%inf_sample).is_normal());
        REQUIRE((int_sample%int_sample).is_normal());
        REQUIRE((int_sample%null_sample).is_undef());

        REQUIRE((null_sample%nan_sample).is_nan());
        REQUIRE((null_sample%undef_sample).is_undef());
        REQUIRE((null_sample % inf_sample).is_zero());
        REQUIRE((null_sample % int_sample).is_zero());
        REQUIRE((null_sample%null_sample).is_undef());
    }
}

TEST_CASE("binary shifts") {
    SECTION("left shift") {
        REQUIRE(51785467817564786781467814_bi<<0_bi==51785467817564786781467814_bi);
        REQUIRE(1_bi<<1_bi==2_bi);
        REQUIRE(0_bi<<5982367_bi==0_bi);
        REQUIRE(532489756343248793268747385984725478_bi<<8532984_bi==(532489756343248793268747385984725478_bi*2_bi)<<8532983_bi);
        REQUIRE(18446744073709551616_bi << 1_bi == 36893488147419103232_bi);

        REQUIRE((nan_sample<<nan_sample).is_nan());
        REQUIRE((nan_sample<<undef_sample).is_nan());
        REQUIRE((nan_sample<<inf_sample).is_nan());
        REQUIRE((nan_sample<<int_sample).is_nan());
        REQUIRE((nan_sample<<null_sample).is_nan());

        REQUIRE((undef_sample<<nan_sample).is_nan());
        REQUIRE((undef_sample<<undef_sample).is_undef());
        REQUIRE((undef_sample<<inf_sample).is_undef());
        REQUIRE((undef_sample<<int_sample).is_undef());
        REQUIRE((undef_sample<<null_sample).is_undef());

        REQUIRE((inf_sample<<nan_sample).is_nan());
        REQUIRE((inf_sample<<undef_sample).is_undef());
        REQUIRE((inf_sample<<inf_sample).is_undef());
        REQUIRE((inf_sample<<int_sample).is_infinity());
        REQUIRE((inf_sample<<null_sample).is_infinity());

        REQUIRE((int_sample<<nan_sample).is_nan());
        REQUIRE((int_sample<<undef_sample).is_undef());
        REQUIRE((int_sample<<inf_sample).is_undef());
        REQUIRE((int_sample<<int_sample).is_normal());
        REQUIRE((int_sample<<null_sample).is_normal());

        REQUIRE((null_sample<<nan_sample).is_nan());
        REQUIRE((null_sample<<undef_sample).is_undef());
        REQUIRE((null_sample<<inf_sample).is_undef());
        REQUIRE((null_sample << int_sample).is_zero());
        REQUIRE((null_sample << null_sample).is_zero());
    }
    SECTION("right shift") {
        REQUIRE(0_bi >> 100_bi == 0_bi);
        REQUIRE(32_bi >> 2_bi == 8_bi);
        REQUIRE(100_bi >> 3_bi == 12_bi);
        REQUIRE(36893488147419103232_bi >> 1_bi == 18446744073709551616_bi);

        REQUIRE(51785467817564786781467814_bi<<0_bi==51785467817564786781467814_bi);
        REQUIRE(1_bi<<1_bi==2_bi);
        REQUIRE(0_bi<<5982367_bi==0_bi);
        REQUIRE(532489756343248793268747385984725478_bi<<8532984_bi==(532489756343248793268747385984725478_bi*2_bi)<<8532983_bi);
        REQUIRE(18446744073709551616_bi << 1_bi == 36893488147419103232_bi);

        REQUIRE((nan_sample>>nan_sample).is_nan());
        REQUIRE((nan_sample>>undef_sample).is_nan());
        REQUIRE((nan_sample>>inf_sample).is_nan());
        REQUIRE((nan_sample>>int_sample).is_nan());
        REQUIRE((nan_sample>>null_sample).is_nan());

        REQUIRE((undef_sample>>nan_sample).is_nan());
        REQUIRE((undef_sample>>undef_sample).is_undef());
        REQUIRE((undef_sample>>inf_sample).is_undef());
        REQUIRE((undef_sample>>int_sample).is_undef());
        REQUIRE((undef_sample>>null_sample).is_undef());

        REQUIRE((inf_sample>>nan_sample).is_nan());
        REQUIRE((inf_sample>>undef_sample).is_undef());
        REQUIRE((inf_sample>>inf_sample).is_undef());
        REQUIRE((inf_sample>>int_sample).is_infinity());
        REQUIRE((inf_sample>>null_sample).is_infinity());

        REQUIRE((int_sample>>nan_sample).is_nan());
        REQUIRE((int_sample>>undef_sample).is_undef());
        REQUIRE((int_sample>>inf_sample).is_undef());
        REQUIRE((int_sample>>int_sample).is_normal());
        REQUIRE((int_sample>>null_sample).is_normal());

        REQUIRE((null_sample>>nan_sample).is_nan());
        REQUIRE((null_sample>>undef_sample).is_undef());
        REQUIRE((null_sample>>inf_sample).is_undef());
        REQUIRE((null_sample >> int_sample).is_zero());
        REQUIRE((null_sample >> null_sample).is_zero());
    }
}

TEST_CASE("abs and sign") {
    SECTION("abs") {
        REQUIRE(bigint::abs(0_bi)==0_bi);
        REQUIRE(bigint::abs(-100_bi)==100_bi);
        REQUIRE(bigint::abs(275427542765462_bi)==275427542765462_bi);
    }
    SECTION("get sign") {
        REQUIRE(bigint::get_sign(-5_bi/10_bi)==0);
        REQUIRE(bigint::get_sign(532849753296938743987_bi)==1);
        REQUIRE(bigint::get_sign(-532849753296938743987_bi)==-1);
    }
}

TEST_CASE("LOGIC OPERATIONS") {
    SECTION(">") {
        REQUIRE(1_bi>-2184679238468_bi);
        REQUIRE(0_bi>-2184679238468_bi);
        REQUIRE(5348979386347896214787_bi>5348979386347896214786_bi);
        REQUIRE(-1_bi>-2_bi);
    }
    SECTION("<") {
        REQUIRE(-2184679238468_bi<1_bi);
        REQUIRE(-2184679238468_bi<0_bi);
        REQUIRE(5348979386347896214786_bi<5348979386347896214787_bi);
        REQUIRE(-2_bi<-1_bi);
    }
    SECTION(">=") {
        REQUIRE(1_bi>=-2184679238468_bi);
        REQUIRE(0_bi>=-2184679238468_bi);
        REQUIRE(5348979386347896214787_bi>=5348979386347896214786_bi);
        REQUIRE(-1_bi>=-2_bi);
        REQUIRE(0_bi>=0_bi);
        REQUIRE(123_bi>=123_bi);
        REQUIRE(-54328975879344587349_bi>=-54328975879344587349_bi);
    }
    SECTION("<=") {
        REQUIRE(-2184679238468_bi<=1_bi);
        REQUIRE(-2184679238468_bi<=0_bi);
        REQUIRE(5348979386347896214786_bi<=5348979386347896214787_bi);
        REQUIRE(-2_bi<-1_bi);
        REQUIRE(0_bi<=0_bi);
        REQUIRE(123_bi<=123_bi);
        REQUIRE(-54328975879344587349_bi<=-54328975879344587349_bi);
    }
    SECTION("==") {
        REQUIRE(0_bi==0_bi);
        REQUIRE(123_bi==123_bi);
        REQUIRE(-54328975879344587349_bi==-54328975879344587349_bi);
    }
    SECTION("!=") {
        REQUIRE(0_bi!=1_bi);
        REQUIRE(5_bi!=-5_bi);
        REQUIRE(18446744073709551616_bi!=36893488147419103232_bi);
    }
}

TEST_CASE("transform to c++ types") {
    bigint n0=0_bi;
    bigint n1=10_bi;
    bigint n2=2147483647_bi;
    bigint n3=2147483648_bi;
    bigint n4=4294967295_bi;
    bigint n5=4294967296_bi;
    bigint n6=9223372036854775807_bi;
    bigint n7=9223372036854775808_bi;
    bigint n8=18446744073709551615_bi;
    bigint n9=18446744073709551616_bi;

    REQUIRE(n0.fits_int32());
    REQUIRE(n1.fits_int32());
    REQUIRE(n2.fits_int32());
    REQUIRE(!n3.fits_int32());
    REQUIRE(!n4.fits_int32());
    REQUIRE(!n5.fits_int32());
    REQUIRE(!n6.fits_int32());
    REQUIRE(!n7.fits_int32());
    REQUIRE(!n8.fits_int32());
    REQUIRE(!n9.fits_int32());

    REQUIRE(n0.fits_uint32());
    REQUIRE(n1.fits_uint32());
    REQUIRE(n2.fits_uint32());
    REQUIRE(n3.fits_uint32());
    REQUIRE(n4.fits_uint32());
    REQUIRE(!n5.fits_uint32());
    REQUIRE(!n6.fits_uint32());
    REQUIRE(!n7.fits_uint32());
    REQUIRE(!n8.fits_uint32());
    REQUIRE(!n9.fits_uint32());

    REQUIRE(n0.fits_int64());
    REQUIRE(n1.fits_int64());
    REQUIRE(n2.fits_int64());
    REQUIRE(n3.fits_int64());
    REQUIRE(n4.fits_int64());
    REQUIRE(n5.fits_int64());
    REQUIRE(n6.fits_int64());
    REQUIRE(!n7.fits_int64());
    REQUIRE(!n8.fits_int64());
    REQUIRE(!n9.fits_int64());

    REQUIRE(n0.fits_uint64());
    REQUIRE(n1.fits_uint64());
    REQUIRE(n2.fits_uint64());
    REQUIRE(n3.fits_uint64());
    REQUIRE(n4.fits_uint64());
    REQUIRE(n5.fits_uint64());
    REQUIRE(n6.fits_uint64());
    REQUIRE(n7.fits_uint64());
    REQUIRE(n8.fits_uint64());
    REQUIRE(!n9.fits_uint64());

    REQUIRE(n0.as_int32()==0);
    REQUIRE((-n0).as_int32()==0);
    REQUIRE(n1.as_int32()==10);
    REQUIRE((-n1).as_int32()==-10);
    REQUIRE(n2.as_int32()==2147483647);
    REQUIRE((-n2).as_int32()==-2147483647);

    REQUIRE(n0.as_uint32()==0);
    REQUIRE((-n0).as_uint32()==0);
    REQUIRE(n1.as_uint32()==10);
    REQUIRE((-n1).as_uint32()==10);
    REQUIRE(n2.as_uint32()==2147483647);
    REQUIRE((-n2).as_uint32()==2147483647);
    REQUIRE(n3.as_uint32()==2147483648);
    REQUIRE((-n3).as_uint32()==2147483648);
    REQUIRE(n4.as_uint32()==4294967295);
    REQUIRE((-n4).as_uint32()==4294967295);

    REQUIRE(n0.as_int64()==0);
    REQUIRE((-n0).as_int64()==0);
    REQUIRE(n1.as_int64()==10);
    REQUIRE((-n1).as_int64()==-10);
    REQUIRE(n2.as_int64()==2147483647);
    REQUIRE((-n2).as_int64()==-2147483647);
    REQUIRE(n3.as_int64()==2147483648);
    REQUIRE((-n3).as_int64()==-2147483648);
    REQUIRE(n4.as_int64()==4294967295);
    REQUIRE((-n4).as_int64()==-4294967295);
    REQUIRE(n5.as_int64()==4294967296);
    REQUIRE((-n5).as_int64()==-4294967296);
    REQUIRE(n6.as_int64()==9223372036854775807);
    REQUIRE((-n6).as_int64()==-9223372036854775807);

    REQUIRE(n0.as_uint64()==0);
    REQUIRE((-n0).as_uint64()==0);
    REQUIRE(n1.as_uint64()==10);
    REQUIRE((-n1).as_uint64()==10);
    REQUIRE(n2.as_uint64()==2147483647);
    REQUIRE((-n2).as_uint64()==2147483647);
    REQUIRE(n3.as_uint64()==2147483648);
    REQUIRE((-n3).as_uint64()==2147483648);
    REQUIRE(n4.as_uint64()==4294967295);
    REQUIRE((-n4).as_uint64()==4294967295);
    REQUIRE(n5.as_uint64()==4294967296);
    REQUIRE((-n5).as_uint64()==4294967296);
    REQUIRE(n6.as_uint64()==9223372036854775807);
    REQUIRE((-n6).as_uint64()==9223372036854775807);
    REQUIRE(n7.as_uint64()==9223372036854775808);
    REQUIRE((-n7).as_uint64()==9223372036854775808);
    REQUIRE(n8.as_uint64()==18446744073709551615);
    REQUIRE((-n8).as_uint64()==18446744073709551615);
}