# volvo-l120h-drive-control


### Uart validation

#Validate the need for the first line. Should now have to register the group to user.
```shell
sudo usermod -aG uucp $USER 
sudo chmod 666 /dev/ttyUSB1
stty -F /dev/ttyUSB1 115200 raw -echo
echo -n "YourData" > /dev/ttyUSB1   
```


## ISO compliance ##
* ISO 26262 for Automotive (ISO 26262, CAN, LIN)
* DO-178C for Aerospace (ARINC 429, MIL-STD-1553)
* IEC 61508 for General Safety-Critical Systems

## Key Industrial-Grade Features

| Feature             | Current Implementation                                         | Industrial Standard Equivalent                         | Notes                                                 |
|---------------------|----------------------------------------------------------------|------------------------------------------------|------------------------------------------------|
| **Message Framing** | Message framing checking for start (0xAA) and end bytes (0x55) | Sync Word (`0x55AA` in LIN, CAN Start-of-Frame) | ✅ Ensure correct framing with a sync word |
| **Checksum/CRC**    | CRC16-CCITT                                                    | CRC-16 or CRC-32 in CAN, MIL-STD-1553           | ✅ CRC-32 is stronger for high-reliability systems |
| **Payload Integrity** | 16 bytes fixed length payload for processing consistency       | Standard in CAN, MIL-STD-1553                   | ✅ |
| **Error Handling**  | **Not Yet Implemented**                                        | Error detection in CAN, ARINC429                | ❌ **Add CRC Validation, Length Checks, and Start Byte Verification** |
| **Timeout Handling** | **Not Yet Implemented**                                        | Required in ISO 26262, RTCA DO-178C             | ❌ **Add Timeout Handling** – Prevents communication stalls |
| **Retry Mechanism** | **Not Yet Implemented**                                        | Required in LIN, CAN, MIL-STD-1553              | ❌ **Add Retransmission on Failure** – Ensure reliable message delivery |
| **Prioritization**  | **Not Yet Implemented**                                        | Arbitration in CAN, Scheduled Frames in LIN     | ❌ **Add Prioritization (QoS)** – Higher priority messages must be sent first |
| **Security**        | **Not Yet Implemented**                                        | Cryptographic Authentication (ISO 21434, AES in modern protocols) | ❌ **Add Authentication** – Ensure secure communication and prevent tampering |

### Message Framing

| SOF  | Length | Payload  | CRC    | EOF  |
|------|--------|---------|--------|------|
| 0xAA | N      | N bytes | 16-bit | 0x55 |

* SOF (Start of Frame): 0xAA
* Length: Number of bytes in the payload
* Payload: Actual data. Supports payloads up to 16 bytes
* CRC (Cyclic Redundancy Check): 16-bit (CRC-16-CCITT)
* EOF (End of Frame): 0x55 (optional if length and CRC are reliable)