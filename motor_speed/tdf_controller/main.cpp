#include<systemc>
#include<systemc-ams>
#include<iostream>

#include "../motor/dc_motor_tdf.h"
#include "tdf_controller.h"
#include "../common/constants.h"

int sc_main(int argc, char* argv[]){
    sc_core::sc_set_time_resolution(10.0, sc_core::SC_NS);

    sca_tdf::sca_signal<double> motor_speed_1;
    sca_tdf::sca_signal<double> u_tdf;

    // TDF controller 
    tdf_controller controller_tdf("controller_tdf", REF_VAL, KP, KI, KD); 
    controller_tdf.y(motor_speed_1);
    controller_tdf.u(u_tdf); 
    
    dc_motor_tdf dc_motor_1("dc_motor_1"); 
    dc_motor_1.input_voltage(u_tdf);
    dc_motor_1.angular_speed(motor_speed_1);

    // Start tracing a tabular file
    sca_util::sca_trace_file * atf = sca_util::sca_create_tabular_trace_file("trace.dat");
    sca_trace(atf, motor_speed_1 ,"motor_speed_1");
    
    // Start simulation for a given time
    sc_core::sc_start(SIMULATION_TIME_MS, sc_core::SC_MS);

    // Stop tracing
    sca_util::sca_close_tabular_trace_file(atf);

    std::cout << "\n\n*** SIMULATION FINISHED \n";
	std::cout << "*** Simulation results have been written in 'trace.dat'"<< std::endl;
	std::cout << "*** You can run 'octave sim.m' to see them graphically" << std::endl; 

    return 0;
}