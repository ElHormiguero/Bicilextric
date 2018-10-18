# Bicilextric

Las bicicletas están colocadas sobre unos soportes de entrenamiento que tiene un rodillo que ofrece una resistencia regulable, de este modo oponemos resistencia al pedaleo.

La velocidad de la rueda de la bicicleta se obtiene a través de un sensor Hall, un sensor de campo magnético. Al hacer circular una corriente en presencia de un campo magnético, los electrones en movimiento, que originan otro campo magnético, son desviados dando lugar a una tensión perpendicular a la corriente llamada voltaje Hall.

El sensor detecta el paso de dos imanes simétricamente colocados en la rueda trasera, de forma que cada vez que detectamos el paso de un imán, la rueda ha avanzado la mitad de su perímetro. Gracias a un microcontrolador, calculamos el tiempo que pasa entre que detecta dos imanes consecutivos, obteniendo la velocidad como espacio entre el tiempo. La velocidad se refleja sobre un contador analógico realizado con un servomotor y en un panel de 7 segmentos. 

El voltaje en cada una de las pistas del scalextric se regula con un transistor conectado a una fuente de alimentación, de forma directamente proporcional a la velocidad, estableciendo una relación entre la velocidad de pedaleo y la velocidad del coche.
