# Device Trust Shim (DTS)

**Header-only C++17 library for tamper-evident audit logging on safety-critical devices**

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Header-only](https://img.shields.io/badge/Header--only-Yes-green.svg)](https://github.com/BigDataPlumbing/device_trust_shim)

---

## Overview

In the era of AI-connected medical devices, log integrity is non-negotiable. **Device Trust Shim (DTS)** provides a lightweight, deterministic mechanism to ensure device logs are tamper-evident from the moment of generation.

DTS creates a **cryptographic chain** where each log entry includes a hash of the previous entry, making deletion or modification detectable. Designed for **IEC 62304** compliant medical device firmware, DTS integrates seamlessly into embedded systems with minimal overhead.

### Key Features

- **Header-only**: Single-file integration, no external dependencies
- **Tamper-evident**: Cryptographic chain prevents log deletion/modification
- **Deterministic**: Reproducible outputs for audit verification
- **Lightweight**: Minimal memory footprint, suitable for embedded systems
- **Standards-aligned**: Designed for IEC 62304, FDA 21 CFR Part 820 compliance
- **Domain adapters**: Optional adapters for DICOM/PACS, MedTech devices, industrial, and building automation

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         DEVICE TRUST SHIM                           │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────────────────┐ │
│  │ Core Layer  │    │  Adapters   │    │       Examples          │ │
│  │             │    │             │    │                         │ │
│  │ audit_chain │───▶│ dicom       │───▶│ radiology_device        │ │
│  │   .hpp      │    │ medtech     │    │ infusion_pump           │ │
│  │             │    │ clinical    │    │ industrial (PLC/SCADA)  │ │
│  │ SHA-256     │    │ industrial  │    │ building_automation     │ │
│  │ Merkle Chain│    │ building    │    │                         │ │
│  └─────────────┘    └─────────────┘    └─────────────────────────┘ │
│         │                                         │                 │
│         ▼                                         ▼                 │
│  ┌─────────────────────────────────────────────────────────────┐   │
│  │                    JSON Log Output                          │   │
│  │  {"device_id":"...", "chain_hash":"abc123...", ...}         │   │
│  └─────────────────────────────────────────────────────────────┘   │
│                              │                                      │
└──────────────────────────────┼──────────────────────────────────────┘
                               ▼
              ┌────────────────────────────────────┐
              │         Trust Stack Integration    │
              │                                    │
              │  ┌──────────────────────────────┐  │
              │  │    clinical_ai_gateway       │  │
              │  │    (Policy Enforcement)      │  │
              │  └──────────────────────────────┘  │
              │                                    │
              │  ┌──────────────────────────────┐  │
              │  │    opensource_truststack     │  │
              │  │    (Verification Layer)      │  │
              │  └──────────────────────────────┘  │
              └────────────────────────────────────┘
```

---

## Use Cases

### Medical Device Manufacturers

- **PACS Integration**: Radiology devices generating audit trails for FDA compliance
- **Infusion Pumps**: Safety-critical logging for post-market surveillance
- **Ventilators**: Event logging with cryptographic integrity for regulatory audits
- **SaMD (Software as a Medical Device)**: AI-enabled devices requiring verifiable audit trails

### Industrial & Manufacturing

- **PLCs & SCADA**: Rockwell (Allen-Bradley), Siemens, GE, Honeywell systems requiring audit trails
- **Production Systems**: Batch tracking, quality control, recipe management (MES integration)
- **Protocol Logging**: Modbus, OPC UA, EtherNet/IP, Profinet, DNP3 event tracking
- **Safety Systems**: Interlock events, emergency stops, safety-critical operations
- **CMMC Compliance**: Defense contractors requiring immutable OT logs

### Building Automation & Smart Infrastructure

- **HVAC & Energy**: Temperature, humidity, energy consumption tracking
- **Access Control**: Door access, badge reader events (security audit requirements)
- **Lighting Control**: KNX, BACnet lighting system events
- **Fire Safety**: Life safety system events (smoke detectors, sprinklers)
- **Protocol Support**: KNX, BACnet, Modbus building automation protocols

---

## Quick Start

### Installation

DTS is header-only. Simply copy `include/dts/` into your project, or use CMake:

```cmake
# Add DTS to your CMakeLists.txt
add_subdirectory(device_trust_shim)
target_link_libraries(your_target PRIVATE dts::DeviceTrustShim)
```

### Basic Usage

**Core Library:**
```cpp
#include <dts/audit_chain.hpp>

// Initialize with device serial number
dts::AuditChain logger("PACS-2025-001234");

// Log events with tamper-evident chain
std::string entry = logger.log(
    "Patient scan initiated - MRN: 12345678",
    dts::UserID::Operator,
    dts::Severity::Info
);
```

**Domain-Specific Adapters:**

For **Radiology/PACS** devices:
```cpp
#include <dts/adapters/dicom_adapter.hpp>

dts::adapters::DICOMAdapter logger("PACS-2025-001234", "PACS-AE-01");

// Log DICOM study creation with automatic tag extraction
logger.log_study_created(
    "1.2.840.113619.2.55.3.1234567890.1234567890123456",  // StudyInstanceUID
    "ANON-12345678",  // PatientID
    "MR"  // Modality
);

// Log AI inference (required for FDA compliance)
logger.log_ai_inference_request(
    "1.2.840.113619.2.55.3.1234567890.1234567890123456",
    "radiology-detection-v2",
    "2.1.0"
);
```

For **MedTech devices** (infusion pumps, ventilators):
```cpp
#include <dts/adapters/medtech_adapter.hpp>

dts::adapters::MedTechAdapter logger("INFUSION-PUMP-789012");

// Log medication administration
logger.log_medication_event(
    dts::adapters::MedicationEventType::Started,
    "Insulin",
    "100U/mL",
    "2.5 mL/hr",
    60  // duration in minutes
);

// Log safety alarm (IEC 60601-1-8)
logger.log_safety_alarm(
    "Occlusion",
    dts::adapters::AlarmPriority::High,
    "Pressure threshold exceeded"
);
```

For **Clinical Trial** devices:
```cpp
#include <dts/adapters/clinical_trial_adapter.hpp>

dts::adapters::ClinicalTrialAdapter logger("TRIAL-DEVICE-001", "PROTOCOL-2025-001");

// Log patient enrollment (automatic anonymization)
logger.log_patient_enrolled("PATIENT-12345", "SITE-001");

// Log protocol deviation (GxP compliance)
logger.log_protocol_deviation(
    "PATIENT-12345",
    "Missed Visit",
    "Patient missed scheduled visit window by 2 days"
);
```

For **Industrial/Manufacturing** systems (PLCs, SCADA):
```cpp
#include <dts/adapters/industrial_adapter.hpp>

dts::adapters::IndustrialAdapter logger("PLC-AB-1756-L75-001", "ASSET-001", "LINE-A");

// Log PLC I/O change (critical for forensics)
logger.log_io_change("FILL_VALVE_OUT", "0", "1", true);

// Log production batch event
logger.log_production_event(
    dts::adapters::ProductionEventType::BatchStarted,
    "BATCH-2025-001234",
    "PRODUCT-ABC-500ML"
);

// Log protocol event (Modbus, OPC UA, etc.)
logger.log_protocol_event(
    dts::adapters::ProtocolType::Modbus,
    "192.168.1.10:502",
    "192.168.1.20:502",
    "Function 03"
);
```

For **Building Automation** systems (KNX, BACnet):
```cpp
#include <dts/adapters/building_automation_adapter.hpp>

dts::adapters::BuildingAutomationAdapter logger("BAS-CONTROLLER-001", "BUILDING-A");

// Log HVAC event
logger.log_hvac_event("Setpoint Change", "ZONE-3F-01", 22.5, "°C");

// Log access control (security audit)
logger.log_access_control("DOOR-MAIN-ENTRANCE", "BADGE-12345", true);

// Log KNX bus event
logger.log_knx_event("1/2/3", "ON", "DPT1.001");
```

### Verification

```cpp
// Collect log entries (e.g., from device storage or cloud upload)
std::vector<std::string> log_entries = { /* ... */ };

// Verify chain integrity
bool is_valid = dts::AuditChain::verify_chain(log_entries);
if (!is_valid) {
    // Tampering detected - logs have been modified or deleted
}
```

---

## Architecture

### Cryptographic Chain

Each log entry contains:
- **previous_hash**: SHA-256 hash of the previous entry's `chain_hash`
- **chain_hash**: SHA-256 hash of the current entry's payload

This creates an **immutable chain** where:
- Deleting an entry breaks the chain (detectable)
- Modifying an entry changes its hash (detectable)
- Inserting a fake entry breaks the chain (detectable)

### Initialization

The chain starts with a deterministic seed hash (`DTS_INIT`), ensuring reproducible verification across device reboots.

---

## API Reference

### `AuditChain`

```cpp
class AuditChain {
public:
    // Initialize with device identifier
    explicit AuditChain(const std::string& device_id);
    
    // Log an audit event
    std::string log(const std::string& message,
                    UserID user_id = UserID::System,
                    Severity severity = Severity::Info);
    
    // Get current chain hash (for verification)
    std::string get_chain_hash() const;
    
    // Get sequence number (total entries)
    uint64_t get_sequence_number() const;
    
    // Verify chain integrity from log entries
    static bool verify_chain(const std::vector<std::string>& entries);
};
```

### Enumerations

```cpp
enum class UserID : uint8_t {
    System = 0,
    Admin = 1,
    Operator = 2,
    Service = 3,
    Unauthorized = 255
};

enum class Severity : uint8_t {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Critical = 4
};
```

---

## Domain-Specific Adapters

DTS includes **optional adapters** for common medical device domains, providing structured logging APIs tailored to specific use cases:

### DICOM Adapter (`dts/adapters/dicom_adapter.hpp`)

For **PACS and radiology devices**:
- DICOM study/instance creation and modification
- AI inference request/completion tracking (FDA compliance)
- DICOMweb STOW-RS transfer logging
- Access control events (HIPAA audit requirements)
- Automatic extraction of DICOM tags (StudyInstanceUID, PatientID, Modality)

**Use Cases:** MRI/CT scanners, PACS workstations, AI-enabled radiology devices

### MedTech Adapter (`dts/adapters/medtech_adapter.hpp`)

For **safety-critical medical devices**:
- Power-on self-test (POST) logging
- Medication administration events (infusion pumps)
- Safety alarm tracking (IEC 60601-1-8)
- Device calibration and maintenance
- Firmware update events

**Use Cases:** Infusion pumps, ventilators, patient monitors, surgical devices

### Clinical Trial Adapter (`dts/adapters/clinical_trial_adapter.hpp`)

For **clinical trial data collection devices**:
- Patient enrollment and visit tracking
- Data collection events (CRF logging)
- Protocol deviation logging (GxP compliance)
- Adverse event reporting (SAE requirements)
- Automatic patient ID anonymization
- Data export audit trails

**Use Cases:** EDC systems, wearable trial devices, point-of-care trial data collection

### Industrial Adapter (`dts/adapters/industrial_adapter.hpp`)

For **PLCs, SCADA, and manufacturing systems**:
- PLC program execution and I/O change tracking
- SCADA alarm logging
- Production batch tracking (MES integration)
- Protocol event logging (Modbus, OPC UA, EtherNet/IP, Profinet, DNP3)
- Safety interlock events (critical for safety systems)
- Equipment status changes
- Parameter/recipe change audit trails
- Maintenance event logging

**Use Cases:** Rockwell (Allen-Bradley), Siemens S7, GE iFIX/Proficy, Honeywell Experion, manufacturing lines, process control systems

**Compliance:** CMMC, NIST 800-82, ISA/IEC 62443

### Building Automation Adapter (`dts/adapters/building_automation_adapter.hpp`)

For **building automation systems (BAS)**:
- HVAC events (temperature, humidity, setpoints)
- Lighting control (brightness, schedules)
- Access control events (door access, badge readers)
- Fire safety events (smoke detectors, sprinklers)
- Energy consumption tracking
- KNX bus event logging
- BACnet object property changes
- Elevator/lift events
- Security system events (motion detection, intrusion)

**Use Cases:** Smart buildings, KNX systems, BACnet networks, HVAC controllers, access control systems

**Protocols:** KNX, BACnet, Modbus, MQTT

## Examples

### Core Library Examples

- `examples/radiology_device_example.cpp` - Basic audit logging for radiology devices
- `examples/infusion_pump_example.cpp` - Safety-critical device logging

### Adapter Examples

- `examples/dicom_adapter_example.cpp` - DICOM-specific logging for PACS integration
- `examples/clinical_trial_adapter_example.cpp` - GxP-compliant trial data logging
- `examples/industrial_adapter_example.cpp` - PLC/SCADA logging for manufacturing systems
- `examples/building_automation_example.cpp` - KNX/BACnet logging for smart buildings

---

## Building

### Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+ (optional, for examples and tests)

### Build Examples

```bash
mkdir build && cd build
cmake ..
make
./examples/radiology_example
./examples/infusion_pump_example
```

### Run Tests

```bash
cd build
ctest
# or
./tests/test_audit_chain
```

---

## Integration with Trust Stack

DTS is designed to integrate with the **Open Source Trust Stack** ecosystem:

1. **Device Layer**: DTS generates tamper-evident logs on-device
2. **Gateway Layer**: Logs are uploaded to `clinical_ai_gateway` for policy enforcement
3. **Verification Layer**: Chain integrity is verified using `opensource_truststack` tools
4. **Compliance Layer**: Logs feed into compliance dashboards and audit reports

See [opensource_truststack](https://github.com/BigDataPlumbing/opensource_truststack) for the complete reference implementation.

---

## Performance Characteristics

- **Memory**: ~200 bytes per `AuditChain` instance (excluding log storage)
- **CPU**: ~50-100 microseconds per log entry (SHA-256 computation)
- **Storage**: ~300-500 bytes per JSON log entry (depending on message length)
- **Thread Safety**: Single-writer safe (typical for device firmware)

---

## Compliance & Standards

DTS is designed to support:

- **IEC 62304**: Medical device software lifecycle processes
- **FDA 21 CFR Part 820**: Quality system regulation for medical devices
- **IEC 60601-1-8**: Medical electrical equipment alarm systems
- **ISO 13485**: Quality management systems for medical devices

**Note**: DTS provides the technical mechanism for tamper-evident logging. Full compliance requires integration with your quality management system, validation procedures, and regulatory documentation.

---

## Security Considerations

- **Cryptographic Strength**: Uses SHA-256 (NIST-approved hash function)
- **Deterministic**: Same inputs produce same outputs (critical for audit verification)
- **No External Dependencies**: Reduces attack surface, suitable for air-gapped systems
- **Memory Safety**: C++17 with RAII patterns, no manual memory management

**Limitations**:
- Single-writer design (not thread-safe for concurrent writes)
- No encryption of log content (use application-layer encryption if required)
- No automatic log rotation (implement at application layer)

---

## License

MIT License - see [LICENSE](LICENSE) file for details.

---

## Contributing

This is a reference implementation for the HealthSec Alliance Trust Stack. For production deployments, consider:

1. **Hardware Security Module (HSM)**: Use HSM-backed key storage for enhanced security
2. **Log Rotation**: Implement automatic rotation to prevent storage exhaustion
3. **Compression**: Compress log entries before storage/transmission
4. **Encryption**: Add application-layer encryption for sensitive log content

---

## Related Projects

- [opensource_truststack](https://github.com/BigDataPlumbing/opensource_truststack) - Complete Trust Stack reference implementation
- [clinical_ai_gateway](https://github.com/BigDataPlumbing/clinical_ai_gateway) - AI guardrails for HIPAA/FDA compliance
- [secure_crate](https://github.com/BigDataPlumbing/secure_crate) - Cryptographic engine for Rust

---

## Support

For questions, issues, or contributions, please open an issue on GitHub or contact [Big Data Plumbing](https://www.bigdataplumbing.com).

---

**Trust nothing. Prove everything.**

