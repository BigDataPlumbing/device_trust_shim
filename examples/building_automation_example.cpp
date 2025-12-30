/**
 * @file building_automation_example.cpp
 * @brief Example: Building automation audit logging (KNX, BACnet, etc.)
 * 
 * Demonstrates audit logging for building automation systems including
 * HVAC, lighting, access control, and energy management.
 */

#include <dts/adapters/building_automation_adapter.hpp>
#include <iostream>

int main() {
    // Initialize for a building automation controller
    dts::adapters::BuildingAutomationAdapter logger(
        "BAS-CONTROLLER-001",
        "BUILDING-A",  // Building identifier
        "ZONE-3F-01"   // Zone identifier
    );
    
    std::cout << "=== Device Trust Shim - Building Automation Example ===\n\n";
    
    // HVAC event (temperature setpoint change)
    std::cout << logger.log_hvac_event(
        "Setpoint Change",
        "ZONE-3F-01",
        22.5,  // Temperature
        "°C"
    ) << "\n\n";
    
    // Lighting control
    std::cout << logger.log_lighting_event(
        "ZONE-3F-01",
        0,     // Old level (off)
        75,    // New level (75%)
        "schedule"  // Controlled by schedule
    ) << "\n\n";
    
    // Access control (critical for security audit)
    std::cout << logger.log_access_control(
        "DOOR-MAIN-ENTRANCE",
        "BADGE-12345",
        true,  // Granted
        ""
    ) << "\n\n";
    
    // Access denied
    std::cout << logger.log_access_control(
        "DOOR-SERVER-ROOM",
        "BADGE-67890",
        false,  // Denied
        "Unauthorized access level"
    ) << "\n\n";
    
    // Fire safety event (critical)
    std::cout << logger.log_fire_safety_event(
        "Smoke Detector Activation",
        "ZONE-3F-01",
        dts::Severity::Critical
    ) << "\n\n";
    
    // Energy consumption
    std::cout << logger.log_energy_consumption(
        "METER-MAIN-001",
        1250.5,  // kWh
        45.2     // Peak kW
    ) << "\n\n";
    
    // KNX bus event
    std::cout << logger.log_knx_event(
        "1/2/3",  // Group address
        "ON",
        "DPT1.001"  // Switch
    ) << "\n\n";
    
    // BACnet event
    std::cout << logger.log_bacnet_event(
        "Analog Input",
        12345,  // Object instance
        "Present Value",
        "22.5"
    ) << "\n\n";
    
    // Schedule event
    std::cout << logger.log_schedule_event(
        "SCHEDULE-WORK-HOURS",
        "HVAC Mode: Cooling",
        "ZONE-3F-01"
    ) << "\n\n";
    
    // Elevator event
    std::cout << logger.log_elevator_event(
        "ELEVATOR-A",
        "Arrival",
        3  // Floor 3
    ) << "\n\n";
    
    // Security event
    std::cout << logger.log_security_event(
        "Motion Detected",
        "ZONE-3F-01-AFTER-HOURS",
        dts::Severity::Warning
    ) << "\n\n";
    
    std::cout << "Chain Hash: " << logger.get_chain_hash() << "\n";
    
    return 0;
}

