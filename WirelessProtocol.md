# Wireless Communication Protocol 
This protocol uses two wireless UART transmitters attached to usart2 on both VEX robots

## Handshake 
To initiate a communication line between the two parties, a handshake
between the two must occur.  A handshake must occur for *every* transmission
to establish who is talking and who is listening.  A handshake occors when
when party makes a request to the other, and the other party confirms that
request, initiating the line of communication.

## Transmission

#### Single Request
*This requests the receiving party to listen for a single data packet*
- To request a single transmit to the other party, the transmitting party must send
   the SINGLE_REQUEST(0xA5) byte to the receiving party
- The transmitting party must wait to receive one of two responses
  - CONFIRM: Other party is ready to receive transmission
  - DENY: Other party is busy; try again later
  - If the transmission is denied, the transmitting party must save
     the command, and attempt transmission at another time
  - If the transmitting party receives any other byte for a handshake
    response, it results in a FAILED state, as this indicates the parties
    are out of sync.

#### Multi Request
*This requests the receiving party to listen for multiple data packets*
- To request muliple transmissions to the other party, the transmitting party must send
   the MULTI_REQUEST(0xA6) byte to the receiving party.
- Multi Request works much the same way as single request, except that the two parties do
  not reset states after one data packet

## Receiving
- A party must always be looking for a request to them - utilizing a
  separate thread to constantly check for a REQUEST byte.
- Once a request byte is received, the receiving party must always CONFIRM
   unless the party is undergoing any of the following:
  - Undergoing user control
    - parties are under command of humans first and foremost
  - Currently transmitting
  - Executing a previously received command
- If the party is undergoing any of these conditions, a DENY will be sent in
   response to the handshake
  - It is up to the transmitting party to send at another time (For now).
- Once the receiving party done processing the request, an OVER (0x75) byte will be
   transmitted to the transmitting party unless in multi-request mode
- if in multi request mode, the receiving party will endlessly collect and perform
  commands from the transmitting party unless it receives a byte other than a START_BYTE
  at the beginning of a new data packet or if the checksum fails

## Data Packet Structure 
Once a handshake is received and confirmed, the transmitting party
will send this data packet to the receiving party:

### *| Start Byte | Command | PayloadSize | Payload | Checksum |*

- **Start Byte**: Denotes the start of the transmission (0xC3)
- **Command**: Specifies what type of action the receiving party should do (1 byte)
- **PayloadSize**: Specifies in bytes how long the payload is (1 byte)
- **Payload**: Specifies either details for the command, or simply data or sensor
          information from the transmitting party (PayloadSize bytes)
- **Checksum**: An XOR result of the Payload, PayloadSize, Command, and Start Byte
           (1 byte)
