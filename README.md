# generaDeberes
Programa que genera un cuaderno de deberes de matematicas para primaria

Genera por la salida estándar una serie de ejercicios arítmeticos formados por sumas,",restas, multiplicaciones y divisiones, acompañadas de sus respectivas comprobaciones.

El programa genera esas operaciones de acuerdo a las instrucciones que se le pasen desde un fichero de texto en el que es espcifican las operaciones según el siguiente patrón: OP N1 [N2] [:m/MN3] [:l/LN4] dónde OP especifica la operación que se desea generar, pudiendo ser un caracter de los que se especifica a continuación: Suma:'S', 's' o '+'; Resta: 'R', 'r',", '-'; Multiplicación: 'P', 'p', '*', o División: 'D', 'd', '/'. 

A continuación se suministra el número de dígitos para el primer operando y en el caso de multiplicaciones o divisiones, el número de dígitos del segundo operando. 

El tipo de operación especificado se puede repetir de acuerdo con el token 'm' o 'M' (Multiplicador) por <N3> veces. Por defecto, si no se pone nada se genera solo una operación. 

También se puede indicar el valor mínimo de los dígitos que puede contener el número de acuerdo con el token 'l' o 'L', lo que hará que se generen  cifras cuyos dígitos no estén por debajo de <N4>.

Las líneas de definición de operaciones se terminan con punto y coma ";"

Un ejemplo de un fichero de ejercicios es el siguiente:", "\0"};
	# Definición de SUMAS
	S 5:m4:L3; s 6:l5; + 4;
	#Definición de RESTAS"
	R 5:M2:L6; r 3:l6; - 7;	#Admite comentarios con '#'
	#Definición de Productos
	P 4 3:m3:l6; p 6 4:M4:L2; * 5 4;
	#Definición de DIVISIONES
	D 5 2:m3:l6; d 4 3:L6; / 6 2;
