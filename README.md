# osurs

Optimization of space utilization in reservation systems.

The osurs contains the following packages:

- `network.h`: Structures to represent a network of a reservation system.
- `routing.h`: Simple routing alogrithm without transfers for checking seat availability in connections.
- `reserve.h`: Optimizing space utilization in reservation systems.

Note: The core functionality of **osurs** is in the reserve package.
There are already powerful routing algorithms for public transport,
so the algorithm included here is minimal and only serves to book
reservations on already known/found connections to the right segments
of the trips (without transfers).

## To do

- Headers:
  - `types.h`
  - `error.h`
  - `io.h`
  - `optim.h`

## DevDeps

- cmake
- valgrind (`valgrind -s --leak-check=full ./main`)
- doxygen, graphviz
- googletest (installed via cmake)

- **Project Setup**
  - Alternative: [Good structure](https://matgomes.com/integrate-google-test-into-cmake/)
  - C code in CPP: [extern "C"](https://stackoverflow.com/questions/23646595/how-to-use-a-c-file-to-write-a-test-class-in-google-test-instead-of-cpp-file)
  - [open-std](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1204r0.html)

```txt
# Example structure: https://gitlab.com/CLIUtils/modern-cmake/-/tree/master/examples/extended-project
# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.
# set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/osurs/types.h")
# Optionally glob, but only for CMake 3.12 or later:
file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/modern/*.h")
# See: 
# - CTest: https://coderefinery.github.io/cmake-workshop/testing/
# - CTest: https://coderefinery.github.io/cmake-workshop/testing/#test-properties-labels-timeout-and-cost
# - Googletest: https://google.github.io/googletest/quickstart-cmake.html
```

- **Optimierung der Reservationen**:

  - Die Reservationen speichern keine Sitzplatzzuweisungen? --> Wird immer neu berechnet?
  - Reservationen von trips hohlen (Trip ist ein Fahrzeug welches eine Route abfährt, also Annahme Kapazität konstant?)
  - Abbilden von structs in Tobis Magic BitMuster
  - Rotieren und Kaste-Optimere
  - Struct `Vechicle` oder `Container` returnen mit optmiererer Sitzplatz verteilung
  - Container: Kapazität, Sitzplätze und Reservationen, eventuell müssen wir hier IDs einführen. Also Sitz poatz und Reservaitons IDs, ... ist aber Zukunftsmusik.

- **Einfache API bereitstellen**:

  - Ich würde einen Python Wrapper vorschlagen, alle Berechnungen passieren in C, python kennt nur den Pointer zu dem network struct und ruft die C funktionen auf den struct pointer auf.
  - Funktionalität:
    - Network Class mit methoden:
      - net.add_node(name, coords)
      - net.add_route(node_list, trip_list, vehicle_list)
      - connection oder None = net.find_route(nodeA, nodeB, departure, seats) --> gleich mehrere Plätze prüfen, e.g. Familien usw.
      - net.reserve_connection(connection)
      - container/vehicle = net.optimize_utilization(trip): Optimiert platz reservationen (THE MAGIC IS HERE)
      - net.export(): Vorschlag --> Wir exportieren das netzwerk als MATSim XML public transport network, dann kann es auch für simulationen verwendet werden.
      - net.import(): Importiert MATSim XML public transport network, vorteil: Bereits bestehende Netzwerke können importiert werden.
      - Visualiserungen: Netzwerk plotten, auslastung pro route, Auslastung pro Fahrzeug/Trip plotten.
