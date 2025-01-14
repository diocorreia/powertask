#include <stdio.h>
#include <errno.h>

#include <powertask/energy.h>

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                    Private API                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------------------------------ */
/*                                                     Public API                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */

int powertask_get_available_energy(powertask_energy_source_t *energy_source){
    int voltage_mV, capacitance_uF;

    if(energy_source == NULL){
        return -EINVAL;
    }

    if(energy_source->capacitance == 0 || energy_source->get_voltage == NULL){
        return -EINVAL;
    }

    voltage_mV = energy_source->get_voltage();
    capacitance_uF = energy_source->capacitance;

    return capacitance_uF * voltage_mV * voltage_mV * (1/2000000);
}
