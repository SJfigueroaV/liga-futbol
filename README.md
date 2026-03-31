# Sistema de Liga de Fútbol

**Estudiante:** Santiago Figueroa Villada  
**Materia:** Pensamiento algoritmico

## Descripción
Aplicación de consola en C++ que gestiona una liga de fútbol completa.
Lee su configuración desde config.txt, registra partidos, calcula la tabla
de posiciones y lleva historial de jornadas. El estado persiste entre ejecuciones.

## Compilación
```bash
g++ -o liga src/main.cpp
```

## Ejecución
```bash
./liga
```
(En Windows: `liga.exe`)

## Formato de config.txt
Usa pares clave=valor, uno por línea. Las líneas que empiezan con # son comentarios.
```
# Comentario ignorado
liga=Nombre de la liga
puntos_victoria=3
puntos_empate=1
puntos_derrota=0
equipo=Equipo Uno
equipo=Equipo Dos
```

## Decisiones de diseño
- Los puntos por resultado se leen desde config.txt, no están fijos en el código.
- partidos.txt usa | como delimitador para evitar conflictos con espacios en nombres.
- fechas.txt usa bloques JORNADA=N / FIN_JORNADA para facilitar la lectura por secciones.
