#!/bin/bash
if [ $# -ne 2 ]; 
  then
  echo Error: Faltan argumentos
  echo Usage: $0 numero_producores numero_consumidores
  echo P.j: $0 5 5
  exit 1
fi
productores=$1
consumidores=$2

if [ $productores -le 0 ] || [ $consumidores -le 0 ]; then
  echo El número de productores y consumidores debe ser mayor a 0
  exit 1
fi

for (( i = 1; i <= $consumidores; i++ )); do
    echo "~/Desktop/Practica4/OUT/practica4 1 -1 "  
    osascript -e 'tell app "Terminal"
        do script "~/Desktop/Practica4/OUT/practica4 1 -1 "
    end tell'
done

for (( j = 1; j <= $productores; j++ )); do
  letra=$(($j+96))
  echo "~/Desktop/Practica4/OUT/practica4 0 $letra"
  osascript -e 'tell app "Terminal"
      do script "~/Desktop/Practica4/OUT/practica4 0 \"'"$letra"'\""
  end tell'
done