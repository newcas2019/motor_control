#include<systemc>
#include<systemc-ams>
#include<iostream>


#include "../motor/dc_motor_tdf.h"
#include "../common/constants.h"
#include "eln_controller.h"


int sc_main(int argc, char* argv[]){
    sc_core::sc_set_time_resolution(10.0, sc_core::SC_NS);

    sca_tdf::sca_signal<double>  motor_speed_3;
    sca_tdf::sca_signal<double> u_eln;

    // ELN controller
    eln_controller controller_eln("controller_eln", REF_VAL, KP, KI, KD);
    controller_eln.y(motor_speed_3);
    controller_eln.u(u_eln);

    dc_motor_tdf dc_motor_3("dc_motor_3");
    dc_motor_3.input_voltage(u_eln);
    dc_motor_3.angular_speed(motor_speed_3);

    // Start tracing a tabular file
    sca_util::sca_trace_file * atf = sca_util::sca_create_tabular_trace_file("trace.dat");
    sca_trace(atf, motor_speed_3,"motor_speed_3");
    
    // Start simulation for a given time
    sc_core::sc_start(SIMULATION_TIME_MS, sc_core::SC_MS);

    // Stop tracing
    sca_util::sca_close_tabular_trace_file(atf);

    std::cout << "\n\n*** SIMULATION FINISHED \n";
	std::cout << "*** Simulation results have been written in 'trace.dat'"<< std::endl;
	std::cout << "*** You can run 'octave sim.m' to see them graphically" << std::endl; 

    return 0;
}