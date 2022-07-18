# TFG

Este es el codigo creado para mi Trabajo de Fin de Grado "Desarrollo de un sistema de monitorización de flujos RTP 
mediante técnicas heurísticas".

Con esta herramienta podremos monitorizar el tráfico de red sin necesidad de analizar la señalización, 
basándose únicamente en heurísticos para identificar estos flujos.

Estos son los comadnos necesarios para ejecutar el codigo:

gcc hash.c funciones.c listaHash.c consSQL.c listaRTP.c main.c -lpcap -lmysqlclient -lm -o monitorRTP
./ monitorRTP nombrefichero.pcap
