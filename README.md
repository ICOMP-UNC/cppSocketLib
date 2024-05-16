# Echoes of Survival

[![QA Workflow](https://github.com/ICOMP-UNC/cppSocketLib/actions/workflows/QAWorkflow.yml/badge.svg)](https://github.com/ICOMP-UNC/cppSocketLib/actions/workflows/QAWorkflow.yml)

## 🎯 Objectives
- **Develop a C++ library** that encapsulates the complexity of socket programming, providing an easy-to-use interface for sending and receiving data over the network.
- **Ensure the library is robust and fault-tolerant**, capable of handling network inconsistencies and failures gracefully.
- **Create a demo application** using the library to demonstrate basic communication between the refuge and a simulated external server.

## 📝 Evaluation Aspects
1. **Code Quality and Organization**: Code should be well-organized, commented, and adhere to modern C++ standards (C++17 and above).
2. **Functionality**: All functions must perform as specified, with the ability to handle TCP and UDP connections.
3. **Robustness**: The application must handle common network issues like timeouts and interruptions without crashing.
4. **Documentation**: Comprehensive documentation must be provided, detailing how to use the library and the underlying design choices.


## 📽 🎞 Stay connected, stay united...

*Amid the echoing halls of their subterranean refuge, Joel and his team of survivors continue to carve out existence from the clutches of despair. The fungal spores that once drifted through air like deadly whispers now lay dormant, held at bay by the ingenuity and relentless vigilance of those who refuse to succumb. The control room, lit by the glow of screens and the determination of its occupants, buzzes with activity as they implement their new tool of survival—the real-time monitoring application.*

*This application, designed to predict and alert the colony of impending fungal blooms, represents more than just a technological achievement; it symbolizes a flicker of control in a world that has spiraled into chaos. With each alert, the residents can seal off affected sectors, purify airlocks, and safeguard their precarious sanctuary. The software, powered by the rudimentary yet functional network of communication that Joel's team has painstakingly restored, connects them not only to each other but also to distant outposts that have managed to send faint signals through the mire.*

*As Joel oversees the deployment of their latest update, a critical patch to enhance detection algorithms, the screens suddenly flicker. An unexpected transmission breaks through the static—the voice of another leader, from a forgotten refuge rumored to have fallen years ago. This leader, Miriam, speaks of a breakthrough in her colony, a possible cure derived from the very fungus that threatens their existence. The stakes are high, and the path is fraught with peril, but this glimmer of hope propels Joel and his team into action. They must reach Miriam's refuge, not only to retrieve the cure but to unite their fragmented efforts against a common enemy.*

<p align="center">
  <img src="img/sockets.png"/>
</p>

*The journey will be treacherous. Outside the safety of their bunker, the world lies in ruins, patrolled by the afflicted—those unfortunate souls overtaken by the fungus and transformed into mindless carriers of destruction. Yet, the promise of a cure and the chance to reclaim their world from the brink of extinction drives Joel and his comrades forward. With their real-time monitoring application guiding their path, avoiding spore hotspots and navigating through the labyrinth of the fallen cities, they embark on their most crucial mission yet: to bring back hope, to heal the land, and to forge a future from the ruins of the past.*

### Application Description:
1. **Dual Protocol Support:** Echoes of Survival provides full support for both TCP (Transmission Control Protocol) and UDP (User Datagram Protocol), allowing developers to choose the appropriate protocol based on the reliability and speed requirements of their applications.
2. **IPv4 and IPv6 Compatibility:** The library is engineered to seamlessly work with both IPv4 and IPv6, ensuring broad compatibility and future-proofing applications against the evolving Internet infrastructure.
3. **Error Handling:** Advanced error management techniques are integrated to ensure robust operation, with comprehensive error reporting and resolution capabilities to handle network anomalies effectively.
4. **Logging System:** A built-in logging system helps in monitoring and debugging network activities. It records detailed logs of all transactions, which are crucial for troubleshooting and ensuring the reliability of communication.
5. **Security Features:** Security measures are embedded within the library to protect data integrity and confidentiality over networks, essential for maintaining secure communications in vulnerable environments.


## Tasks to Implement
1. **Design the Library Interface**:
   - Define classes and functions for managing socket connections.
   - Include methods for setting up, sending, and receiving data.

2. **Implement TCP and UDP Support**:
   - Develop classes that handle TCP connections, ensuring reliable transmission.
   - Implement UDP communication capabilities, suitable for less critical data that can tolerate loss.

3. **Implement IPv4 and IPv6 Support**:
   - Dual-Stack Compatibility: Ensure that the socket classes are compatible with both IPv4 and IPv6, allowing seamless operation across different network stacks.
   - Flexible Address Handling: Provide mechanisms in the interface to handle both IP address standards transparently.

4. **Error Handling Mechanisms**:
   - Incorporate error handling to manage and recover from network failures.
   - Use exceptions to notify users of unrecoverable errors.

5. **Logging System**:
   - Implement a logging system to record all network transactions, useful for debugging and monitoring the application's behavior.

6. **Create a Demo Application**:
   - Develop a simple command-line application that uses the library to send and receive messages.
   - Include features to connect to a server, send messages, and display responses.

7. **Documentation**:
   - Write clear, concise documentation for the library.
   - Provide examples and usage scenarios to help future developers understand and effectively use your library.
