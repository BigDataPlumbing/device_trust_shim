/**
 * @file industrial_adapter_example.cpp
 * @brief Example: Industrial/OT audit logging for manufacturing systems
 * 
 * Demonstrates audit logging for PLCs, SCADA, and manufacturing equipment
 * (Rockwell, Siemens, GE, etc.) with protocol event tracking.
 */

#include <dts/adapters/industrial_adapter.hpp>
#include <iostream>

int main() {
    // Initialize for a production line (e.g., Rockwell ControlLogix PLC)
    dts::adapters::IndustrialAdapter logger(
        "PLC-AB-1756-L75-001",
        "ASSET-PLC-001",  // Asset tag
        "LINE-A"          // Production line
    );
    
    std::cout << "=== Device Trust Shim - Industrial Adapter Example ===\n\n";
    
    // PLC program execution
    std::cout << logger.log_plc_event(
        "MainProgram",
        42,  // Rung number
        "Bottle fill sequence initiated"
    ) << "\n\n";
    
    // I/O change (critical for forensics)
    std::cout << logger.log_io_change(
        "FILL_VALVE_OUT",
        "0",
        "1",
        true  // Output
    ) << "\n\n";
    
    // SCADA alarm
    std::cout << logger.log_scada_alarm(
        "ALM-001",
        "High temperature detected in Zone 3",
        dts::Severity::Warning,
        false  // Not yet acknowledged
    ) << "\n\n";
    
    // Production batch event
    std::cout << logger.log_production_event(
        dts::adapters::ProductionEventType::BatchStarted,
        "BATCH-2025-001234",
        "PRODUCT-ABC-500ML",
        0
    ) << "\n\n";
    
    // Protocol event (Modbus)
    std::cout << logger.log_protocol_event(
        dts::adapters::ProtocolType::Modbus,
        "192.168.1.10:502",
        "192.168.1.20:502",
        "Function 03 (Read Holding Registers)",
        true
    ) << "\n\n";
    
    // Protocol event (OPC UA)
    std::cout << logger.log_protocol_event(
        dts::adapters::ProtocolType::OPCUA,
        "opc.tcp://plc.example.com:4840",
        "opc.tcp://scada.example.com:4840",
        "Read Node ns=2;s=Temperature",
        true
    ) << "\n\n";
    
    // Safety interlock (critical)
    std::cout << logger.log_safety_interlock(
        "INTERLOCK-EMERGENCY-STOP",
        true,  // Triggered
        "Emergency stop button pressed"
    ) << "\n\n";
    
    // Equipment status change
    std::cout << logger.log_equipment_status(
        "FILLER-STATION-01",
        "Running",
        "Stopped"
    ) << "\n\n";
    
    // Parameter change (recipe)
    std::cout << logger.log_parameter_change(
        "FILL_VOLUME_SETPOINT",
        "500",
        "750",
        dts::UserID::Operator
    ) << "\n\n";
    
    // Quality check
    std::cout << logger.log_production_event(
        dts::adapters::ProductionEventType::QualityCheckPassed,
        "BATCH-2025-001234",
        "PRODUCT-ABC-500ML",
        1000  // Quantity
    ) << "\n\n";
    
    std::cout << "Chain Hash: " << logger.get_chain_hash() << "\n";
    
    return 0;
}

