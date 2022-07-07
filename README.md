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

- **Routen** 
    - Nodes müssen Routen kennen
    - find_connection(&network, &nodeA, &nodeB, departure, seats (min 1!)):
        - Routen die nodeA und nodeB haben filtern.
        - Für jeden trip auf allen selektierten Routen prüfen ob er am nodeA zur departure time schon vorbei ist.
            - Falls ja skip...
            - Falls nein
                - Berechne route (stop orig --> stop dest, departure und ankunftszeit behalten)
                - Prüfe ob noch kapazität entlang stops, falls nicht: Skip.
        - return beste Connection (frühste Ankunfstzeit, welche noch Platz hat)

- **Reservationen**
    - Jeder stop braucht einen reservation counter `reserved` für das folgende Segment, nur relevant für Routing
    - Reservationsvorgang:
        - Die Connection aus dem Routing wird via Referenz übergeben.
        - Es wird erneut geprüft ob Connection noch vorhanden
        - Falls nein: Abbruch
        - Falls Ja:
            - Reservations struct erstellen und Pointer darauf in Array auf Trip speichern.
            - Counter auf stops hochzählen
        - Achtung! Das Ganze (prüfen und verbuchen) braucht Absicherung in in nebenläufiger Ausführung.

- **Optimierung der Reservationen**
    - Die Reservationen speichern keine Sitzplatzzuweisungen? --> Wird immer neu berechnet?
    - Reservationen von trips hohlen (Trip ist ein Fahrzeug welches eine Route abfährt, also Annahme Kapazität konstant?)
    - Abbilden von structs in Tobis Magic BitMuster
    - Rotieren und Kaste-Optimere
    - Struct `Vechicle` oder `Container` returnen mit optmiererer Sitzplatz verteilung
    - Container: Kapazität, Sitzplätze und Reservationen, eventuell müssen wir hier IDs einführen. Also Sitz poatz und Reservaitons IDs, ... ist aber Zukunftsmusik.

- **Einfache API bereitstellen**
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
