#include "bigfloat.h"
#include "bigint.h"
#include <catch2/catch_test_macros.hpp>

using namespace aak;

TEST_CASE("basic operations") {
    SECTION("addition") {
        REQUIRE(1_bf + 1_bf == 2_bf);
        REQUIRE(1_bf + 0_bf == 1_bf);
        REQUIRE(0_bf + 1_bf == 1_bf);
        REQUIRE(0_bf + 0_bf == 0_bf);
        REQUIRE(1_bf + (-1_bf) == 0_bf);
        REQUIRE(1.5_bf+1.5_bf==3_bf);
        REQUIRE(1.5_bf+(-1.5_bf)==0_bf);
        REQUIRE(1.5_bf+1.25_bf==2.75_bf);
    }
    SECTION("subtraction") {
        REQUIRE(1_bf - 1_bf == 0_bf);
        REQUIRE(1_bf - 0_bf == 1_bf);
        REQUIRE(0_bf - 1_bf == (-1_bf));
        REQUIRE(0_bf - 0_bf == 0_bf);
        REQUIRE(1_bf - (-1_bf) == 2_bf);
        REQUIRE(1.5_bf-1.5_bf==0_bf);
        REQUIRE(1.5_bf-(-1.5_bf)==3_bf);
        REQUIRE(1.5_bf-1.25_bf==0.25_bf);
    }
    SECTION("multiplication") {
        REQUIRE(1_bf * 1_bf == 1_bf);
        REQUIRE(1_bf * 0_bf == 0_bf);
        REQUIRE(0_bf * 1_bf == 0_bf);
        REQUIRE(0_bf * 0_bf == 0_bf);
        REQUIRE(1_bf * (-1_bf) == (-1_bf));
        REQUIRE(1.5_bf*1.5_bf==2.25_bf);
        REQUIRE(1.5_bf*(-1.5_bf)==(-2.25_bf));
        REQUIRE(1.5_bf*1.25_bf==1.875_bf);
    }
    SECTION("division") {
        REQUIRE(1_bf / 1_bf == 1_bf);
        REQUIRE(0_bf / 1_bf == 0_bf);
        REQUIRE(1_bf / (-1_bf) == (-1_bf));
        REQUIRE(1.5_bf/1.5_bf==1_bf);
        REQUIRE(1.5_bf/(-1.5_bf)==(-1_bf));
        REQUIRE(1.5_bf/1.25_bf==1.2_bf);
    }
}

TEST_CASE("LOGIC OPERATIONS") {
    SECTION("EQUALITY") {
        REQUIRE(1_bf == 1_bf);
        REQUIRE(1_bf != 0_bf);
        REQUIRE(1.5_bf==1.5_bf);
        REQUIRE(1.5_bf!=1.25_bf);
    }
    SECTION("LESS") {
        REQUIRE(1_bf < 2_bf);
        REQUIRE(1_bf < 1.5_bf);
        REQUIRE(1.5_bf<1.75_bf);
    }
    SECTION("GREATER") {
        REQUIRE(2_bf > 1_bf);
        REQUIRE(1.5_bf > 1_bf);
        REQUIRE(1.75_bf>1.5_bf);
    }
    SECTION("LESS OR EQUAL") {
        REQUIRE(1_bf <= 2_bf);
        REQUIRE(1_bf <= 1.5_bf);
        REQUIRE(1.5_bf<=1.75_bf);
        REQUIRE(1.5_bf<=1.5_bf);
    }
    SECTION("GREATER OR EQUAL") {
        REQUIRE(2_bf >= 1_bf);
        REQUIRE(1.5_bf >= 1_bf);
        REQUIRE(1.75_bf>=1.5_bf);
        REQUIRE(1.5_bf>=1.5_bf);
    }
}

TEST_CASE("OTHER") {
    SECTION("ABS") {
        REQUIRE(bigfloat::abs(1_bf) == 1_bf);
        REQUIRE(bigfloat::abs(0_bf) == 0_bf);
        REQUIRE(bigfloat::abs(-1_bf) == 1_bf);
        REQUIRE(bigfloat::abs(1.5_bf) == 1.5_bf);
        REQUIRE(bigfloat::abs(-1.5_bf) == 1.5_bf);
    }
    SECTION("CUT") {
        REQUIRE(bigfloat::cut(1.5_bf) == 1_bi);
        REQUIRE(bigfloat::cut(1.75_bf) == 1_bi);
        REQUIRE(bigfloat::cut(1.25_bf) == 1_bi);
    }
    SECTION("GET SIGN") {
        REQUIRE(bigfloat::get_sign(1_bf) == 1);
        REQUIRE(bigfloat::get_sign(0_bf) == 0);
        REQUIRE(bigfloat::get_sign(-1_bf) == -1);
        REQUIRE(bigfloat::get_sign(1.5_bf) == 1);
        REQUIRE(bigfloat::get_sign(-1.5_bf) == -1);
    }
    SECTION("DECIMAL") {
        REQUIRE(bigfloat(2187654297.521899247569184658745414_bf).decimal(10) == std::string("2187654297.5218992476"));
    }
}