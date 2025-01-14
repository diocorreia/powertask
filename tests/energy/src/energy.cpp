#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <stdio.h>
#include <stdbool.h>

extern "C"
{
	#include <powertask/energy.h>
}

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                               Test groups declaration                                              */
/* ------------------------------------------------------------------------------------------------------------------ */

TEST_GROUP(test_energy_regular){
	void setup(){

	}

	void teardown(){
		mock().clear();
	}
};

/** @brief Mock get voltage function behaviour */
int fake_get_voltage(void){
    return mock().actualCall("get_voltage").returnIntValue();
}

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                        Unit Tests - test_energy_regular                                            */
/* ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Energy - Get available energy with NULL parameter
 * 
 * The scope of this unit test is to validate the correct behavior of the
 * function when no energy source structure is given.
 * 
 * Is is expected to return an error code.
 */
TEST(test_energy_regular, test_energy_get_available_energy_null_param){
    CHECK(powertask_get_available_energy(NULL) < 0);
}

/**
 * @brief Energy - Get available energy with not initialized parameter
 * 
 * The scope of this unit test is to validate the correct behavior of the
 * function when the energy source structure is not correctly initialized.
 * 
 * Is is expected to return an error code.
 */
TEST(test_energy_regular, test_energy_get_available_energy_not_set){
    powertask_energy_source_t energy_source = {0};

    CHECK(powertask_get_available_energy(&energy_source) < 0);

    energy_source.capacitance = 1;

    CHECK(powertask_get_available_energy(&energy_source) < 0);

    energy_source.capacitance = 0;
    energy_source.get_voltage = fake_get_voltage;

    CHECK(powertask_get_available_energy(&energy_source) < 0);
}

/**
 * @brief Energy - Get available energy
 * 
 * The scope of this unit test is to validate the correct behavior of the function.
 * 
 * Is is expected the return value to match with the expected value.
 */
TEST(test_energy_regular, test_energy_get_available){
    const int fake_voltage = 3;
    const int fake_capacitance = 1; 
    const int expected_available_energy = (1/2) * fake_capacitance * fake_voltage * fake_voltage;

    powertask_energy_source_t energy_source = {
        .capacitance = fake_capacitance,
        .get_voltage = fake_get_voltage,
    };

    mock().expectOneCall("get_voltage").andReturnValue(fake_voltage);
    CHECK_EQUAL(powertask_get_available_energy(&energy_source), expected_available_energy);
}