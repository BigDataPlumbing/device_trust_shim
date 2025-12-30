/**
 * @file building_automation_adapter.hpp
 * @brief Building automation audit logging adapter (KNX, BACnet, etc.)
 * 
 * Provides structured logging for building automation systems (BAS), including
 * HVAC, lighting, access control, and energy management events.
 * 
 * Supports KNX, BACnet, Modbus, and other building automation protocols.
 * 
 * @copyright Copyright (c) 2025 Big Data Plumbing
 * @license MIT License
 */

#ifndef DTS_BUILDING_AUTOMATION_ADAPTER_HPP
#define DTS_BUILDING_AUTOMATION_ADAPTER_HPP

#include "../audit_chain.hpp"
#include <string>
#include <sstream>

namespace dts {
namespace adapters {

/**
 * @brief Building system types
 */
enum class BuildingSystemType : uint8_t {
    HVAC = 1,
    Lighting = 2,
    AccessControl = 3,
    FireSafety = 4,
    EnergyManagement = 5,
    Elevator = 6,
    Security = 7,
    Other = 255
};

/**
 * @brief Building automation audit logger
 * 
 * Extends AuditChain with building automation-specific logging
 * for KNX, BACnet, and other BAS protocols.
 */
class BuildingAutomationAdapter {
public:
    explicit BuildingAutomationAdapter(const std::string& device_id,
                                      const std::string& building_id = "",
                                      const std::string& zone_id = "")
        : chain_(device_id), building_id_(building_id), zone_id_(zone_id) {}
    
    /**
     * @brief Log HVAC event (temperature, humidity, airflow)
     * @param event_type Event type (setpoint change, mode change, etc.)
     * @param zone Zone identifier
     * @param value Value (temperature, humidity %, etc.)
     * @param unit Unit of measurement
     */
    std::string log_hvac_event(const std::string& event_type,
                              const std::string& zone,
                              double value,
                              const std::string& unit = "") {
        std::ostringstream msg;
        msg << "HVAC Event | "
            << "Type:" << event_type << " | "
            << "Zone:" << zone << " | "
            << "Value:" << value;
        
        if (!unit.empty()) {
            msg << " " << unit;
        }
        if (!zone_id_.empty() && zone_id_ != zone) {
            msg << " | ZoneID:" << zone_id_;
        }
        
        return chain_.log(msg.str(), UserID::Operator, Severity::Info);
    }
    
    /**
     * @brief Log lighting control event
     * @param zone Zone/room identifier
     * @param old_level Previous brightness level (0-100%)
     * @param new_level New brightness level (0-100%)
     * @param control_source Source of control (manual, schedule, sensor)
     */
    std::string log_lighting_event(const std::string& zone,
                                  int old_level,
                                  int new_level,
                                  const std::string& control_source = "manual") {
        std::ostringstream msg;
        msg << "Lighting Control | "
            << "Zone:" << zone << " | "
            << "From:" << old_level << "% | "
            << "To:" << new_level << "% | "
            << "Source:" << control_source;
        
        return chain_.log(msg.str(), UserID::Operator, Severity::Info);
    }
    
    /**
     * @brief Log access control event (critical for security audit)
     * @param door_id Door/access point identifier
     * @param user_id User identifier (card number, badge ID, etc.)
     * @param granted Whether access was granted
     * @param reason Reason for denial (if denied)
     */
    std::string log_access_control(const std::string& door_id,
                                  const std::string& user_id,
                                  bool granted,
                                  const std::string& reason = "") {
        std::ostringstream msg;
        msg << "Access Control | "
            << "Door:" << door_id << " | "
            << "User:" << user_id << " | "
            << "Granted:" << (granted ? "Yes" : "No");
        
        if (!granted && !reason.empty()) {
            msg << " | Reason:" << reason;
        }
        
        return chain_.log(msg.str(), UserID::System,
                         granted ? Severity::Info : Severity::Warning);
    }
    
    /**
     * @brief Log fire safety event (critical for life safety)
     * @param event_type Event type (alarm, detector activation, sprinkler, etc.)
     * @param location Location/zone identifier
     * @param severity Event severity
     */
    std::string log_fire_safety_event(const std::string& event_type,
                                    const std::string& location,
                                    Severity severity = Severity::Critical) {
        std::ostringstream msg;
        msg << "Fire Safety Event | "
            << "Type:" << event_type << " | "
            << "Location:" << location;
        
        if (!zone_id_.empty()) {
            msg << " | Zone:" << zone_id_;
        }
        
        return chain_.log(msg.str(), UserID::System, severity);
    }
    
    /**
     * @brief Log energy consumption event
     * @param meter_id Energy meter identifier
     * @param consumption_kwh Energy consumption in kWh
     * @param peak_demand_kw Peak demand in kW (if applicable)
     */
    std::string log_energy_consumption(const std::string& meter_id,
                                     double consumption_kwh,
                                     double peak_demand_kw = 0.0) {
        std::ostringstream msg;
        msg << "Energy Consumption | "
            << "Meter:" << meter_id << " | "
            << "Consumption:" << consumption_kwh << " kWh";
        
        if (peak_demand_kw > 0.0) {
            msg << " | Peak:" << peak_demand_kw << " kW";
        }
        if (!building_id_.empty()) {
            msg << " | Building:" << building_id_;
        }
        
        return chain_.log(msg.str(), UserID::System, Severity::Info);
    }
    
    /**
     * @brief Log KNX bus event
     * @param group_address KNX group address
     * @param data_value Data value sent/received
     * @param data_type Data type (DPT - Datapoint Type)
     */
    std::string log_knx_event(const std::string& group_address,
                             const std::string& data_value,
                             const std::string& data_type = "") {
        std::ostringstream msg;
        msg << "KNX Event | "
            << "GroupAddress:" << group_address << " | "
            << "Value:" << data_value;
        
        if (!data_type.empty()) {
            msg << " | DPT:" << data_type;
        }
        
        return chain_.log(msg.str(), UserID::System, Severity::Info);
    }
    
    /**
     * @brief Log BACnet event
     * @param object_type BACnet object type
     * @param object_instance Object instance number
     * @param property_name Property name
     * @param value Property value
     */
    std::string log_bacnet_event(const std::string& object_type,
                                uint32_t object_instance,
                                const std::string& property_name,
                                const std::string& value) {
        std::ostringstream msg;
        msg << "BACnet Event | "
            << "ObjectType:" << object_type << " | "
            << "Instance:" << object_instance << " | "
            << "Property:" << property_name << " | "
            << "Value:" << value;
        
        return chain_.log(msg.str(), UserID::System, Severity::Info);
    }
    
    /**
     * @brief Log schedule/automation event
     * @param schedule_name Schedule identifier
     * @param action Action taken (on, off, setpoint change, etc.)
     * @param zone Zone affected
     */
    std::string log_schedule_event(const std::string& schedule_name,
                                  const std::string& action,
                                  const std::string& zone = "") {
        std::ostringstream msg;
        msg << "Schedule Event | "
            << "Schedule:" << schedule_name << " | "
            << "Action:" << action;
        
        if (!zone.empty()) {
            msg << " | Zone:" << zone;
        }
        
        return chain_.log(msg.str(), UserID::System, Severity::Info);
    }
    
    /**
     * @brief Log elevator/lift event
     * @param elevator_id Elevator identifier
     * @param event_type Event type (call, arrival, door open/close, etc.)
     * @param floor Floor number
     */
    std::string log_elevator_event(const std::string& elevator_id,
                                  const std::string& event_type,
                                  int floor = -1) {
        std::ostringstream msg;
        msg << "Elevator Event | "
            << "Elevator:" << elevator_id << " | "
            << "Type:" << event_type;
        
        if (floor >= 0) {
            msg << " | Floor:" << floor;
        }
        if (!building_id_.empty()) {
            msg << " | Building:" << building_id_;
        }
        
        return chain_.log(msg.str(), UserID::System, Severity::Info);
    }
    
    /**
     * @brief Log security system event (intrusion detection, cameras, etc.)
     * @param event_type Event type (motion detected, door forced, etc.)
     * @param location Location/zone identifier
     * @param severity Event severity
     */
    std::string log_security_event(const std::string& event_type,
                                  const std::string& location,
                                  Severity severity = Severity::Warning) {
        std::ostringstream msg;
        msg << "Security Event | "
            << "Type:" << event_type << " | "
            << "Location:" << location;
        
        if (!zone_id_.empty()) {
            msg << " | Zone:" << zone_id_;
        }
        
        return chain_.log(msg.str(), UserID::System, severity);
    }
    
    /**
     * @brief Get underlying audit chain
     */
    AuditChain& get_chain() { return chain_; }
    const AuditChain& get_chain() const { return chain_; }
    
    /**
     * @brief Get current chain hash
     */
    std::string get_chain_hash() const {
        return chain_.get_chain_hash();
    }

private:
    AuditChain chain_;
    std::string building_id_;
    std::string zone_id_;
};

} // namespace adapters
} // namespace dts

#endif // DTS_BUILDING_AUTOMATION_ADAPTER_HPP

