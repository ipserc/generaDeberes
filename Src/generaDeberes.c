
#define FALSE 0
#define TRUE 1
#define _DEBUG_ FALSE
#define MAXLIN 80
#define DELIM " \t\n"
#define OP_DELIM ';'
#define TOK_DELIM ":"
#define MULT_DELIM "Mm"
#define DIGLIM_DELIM "Ll"
#define COMMENT '#'

#define byte unsigned char
#define boolean unsigned char

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <string.h>
#include <errno.h>

//Variables externas
extern int errno;

// Variables Globales
int gERRNO;
char* appErr[] = { 	"NO ERROR", \
					"Error de sintáxis en el fichero de definición de ejercicios, línea:",\
					"\0"};

int raiseError( int errNbr, int info1 ) {
	char errorMessage[MAXLIN+1];

	if ( errNbr >0 ) {
		strncpy( errorMessage, strerror(errNbr), MAXLIN ) ;
	} else {
		sprintf( errorMessage, "%s%i", appErr[-errNbr], info1 ) ;
	}

	fprintf( stderr, "Error:%d: - %s\n", errNbr, strerror(errNbr) );
	fflush( stderr );
	exit( errNbr );
}

void randomize( void ) {
	srand( time( NULL ) );
}

void usage( void ) {
	char **ptr;
	char *ejemplo[] = {	"	# Definición de SUMAS",\
						"	S 5:m4:L3; s 6:l5; + 4;",\
						"	#Definición de RESTAS",\
						"	R 5:M2:L6; r 3:l6; - 7;	#Admite comentarios con '#'",\
						"	#Definición de Productos",\
						"	P 4 3:m3:l6; p 6 4:M4:L2; * 5 4;",\
						"	#Definición de DIVISIONES",\
						"	D 5 2:m3:l6; d 4 3:L6; / 6 2;",\
						"\0"};

	char *texto[] = {	"Genera por la salida estándar una serie de ejercicios arítmeticos formados por sumas,",
						"restas, multiplicaciones y divisiones, acompañadas de sus respectivas comprobaciones.",
						"El programa genera esas operaciones de acuerdo a las instrucciones que se le pasen",
						"desde un fichero de texto en el que es espcifican las operaciones según el siguiente",
						"patrón: OP N1 [N2] [:m/MN3] [:l/LN4] dónde OP especifica la operación que se desea generar, pudiendo",
						"ser un caracter de los que se especifica a continuación: Suma:'S', 's' o '+'; Resta: 'R', 'r',",
						"'-'; Multiplicación: 'P', 'p', '*', o División: 'D', 'd', '/'. A continuación se suministra",
						"el número de dígitos para el primer operando y en el caso de multiplicaciones o divisiones,",
						"el número de dígitos del segundo operando. El tipo de operación especificado se puede repetir",
						"de acuerdo con el token 'm' o 'M' (Multiplicador) por <N3> veces. Por defecto, si no se pone nada",
						"se genera solo una operación. También se puede indicar el valor mínimo de los dígitos que",
						"puede contener el número de acuerdo con el token 'l' o 'L', lo que hará que se generen ",
						"cifras cuyos dígitos no estén por debajo de <N4>."
						"Un ejemplo de un fichero de ejercicios es el siguiente:", "\0"};

	puts("");
	puts("Usage: generaDeberes <ficheroEspecificacion>/-?");
	ptr = texto; while( **ptr ) puts( *ptr++ );
	ptr = ejemplo; while( **ptr ) puts( *ptr++ );
	exit( 0 );
}

void pivota( float* num1, float* num2 ) {
	float pivote;

	pivote = *num1;
	*num2 = *num1;
	*num1 = pivote;
}

int getDigito( int notNull, byte digLimit  ) {
	int digito;

	do {
		digito = (int)floor( (float)rand() / (float)RAND_MAX * 10 );
	} while( ( digito < digLimit ) || ((digito == 0) && notNull) );
	return digito;
}

float generaNum( int numDigitos, byte digLimit ) {
	int i;
	float numero = 0;

	numero =  getDigito( TRUE, digLimit ) ;

	for( i=1; i < numDigitos; ++i) {
		numero *= 10;
		numero += getDigito( FALSE, digLimit );
	}
	return numero ;
}

char* enmascara( char* cadena ) {
	char* ptr;
	boolean enmascarar;

	ptr = cadena;
	enmascarar = (int)fmod( strlen( ptr ), 2 );

	while (*ptr){
		if ( enmascarar ) *ptr = '#';
		enmascarar = !enmascarar;
		++ptr;
	}

	return cadena;
}

char* generaSuma( char* ejercicio, int sizeSuma, byte digLimit ) {
	float sumando1, sumando2, resultado ;
	char s_resultado[MAXLIN + 1];

	sumando1 = generaNum( sizeSuma, digLimit );
	sumando2 = generaNum( sizeSuma, digLimit );
	resultado = sumando1 + sumando2;

	sprintf( s_resultado, "%li", (long int)resultado );
	sprintf( ejercicio, "%i + %i = \t\t(Resultado: %s)", (int)sumando1, (int)sumando2, enmascara( s_resultado ) );

	return ejercicio ;
}

char* generaResta( char* ejercicio, int sizeResta, byte digLimit ) {
	float minuendo, sustraendo, resultado;
	char s_resultado[MAXLIN + 1];

	do {
		minuendo = generaNum( sizeResta, digLimit );
		sustraendo = generaNum( sizeResta, digLimit );
		if (minuendo < sustraendo) pivota( &minuendo, &sustraendo );
		resultado = minuendo - sustraendo;
	} while (resultado < pow(10, sizeResta-1));

	sprintf( s_resultado, "%li", (long int)resultado );
	sprintf( ejercicio, "%i - %i = \t\t(Resultado: %s)", (int)minuendo, (int)sustraendo, enmascara( s_resultado ) );

	return ejercicio ;
}

char* generaMultiplicacion( char* ejercicio, int sizeProd1, int sizeProd2, byte digLimit ) {
	double prod1, prod2, resultado;

	prod1 = generaNum( sizeProd1, digLimit );
	prod2 = generaNum( sizeProd2, digLimit );
	resultado = prod1*prod2;

	sprintf( ejercicio, "%.0f x %.0f = \t\t(Resultado: %.0f)", prod1, prod2, resultado);

	return ejercicio;
}

char* generaDivision( char* ejercicio, int sizeDividendo, int sizeDivisor, byte digLimit ) {
	double dividendo, divisor;
	double resto;
	char resto_s[MAXLIN+1];

	if( sizeDividendo < sizeDivisor) return strcpy( ejercicio, "ERROR: Dividendo menor que divisor.");

	dividendo = generaNum( sizeDividendo, digLimit );
	do {
		divisor = generaNum( sizeDivisor, digLimit );
	} while (divisor == 1 || divisor == dividendo);
	resto = fmod( dividendo, divisor );
	sprintf( resto_s, "%.0f", resto );

	sprintf( ejercicio, "%.0f / %.0f = \t\t(Resto: %s)", dividendo, divisor, (resto == 0)? "Exacto":resto_s );

	return ejercicio;
}

char* centra( char* linea, byte tamanio ){
	char *buffer;
	byte tamaLin;

	tamaLin = strlen( linea );

	if ( tamanio == tamaLin ) return linea;

	if ( !(buffer = calloc( tamanio, sizeof( char ) ) ) ) return NULL;
	memset( buffer, ' ', (size_t)floor((tamanio - tamaLin)/2.0));
	sprintf( buffer, "%s%s", buffer, linea);
	strncpy( linea, buffer, strlen( buffer ) );
	free( buffer );

	return linea;
}

char* parseaOrden( char* orden, char* ejercicio, byte digLimit ) {
	char *buffer;
	byte operador, tama1, tama2;
	byte tamanio;

	tamanio = strlen( orden );
	if ( !(buffer = calloc( tamanio+1, sizeof( char ) ) ) ) {
		gERRNO = errno;
		raiseError( gERRNO, 0 );
	}

	strncpy( buffer, orden, tamanio );
	operador = *strtok( buffer, DELIM );
	tama1 = (byte)atoi((char*)strtok( NULL, DELIM ));

	switch( operador ) {
		case 'S':
		case 's':
		case '+': 	ejercicio = generaSuma( ejercicio, tama1, digLimit );
			break;
		case 'R':
		case 'r':
		case '-': 	ejercicio = generaResta( ejercicio, tama1, digLimit );
			break;
		case 'P':
		case 'p':
		case '*': 	tama2 = (byte)atoi((char*)strtok( NULL, DELIM ));
					ejercicio = generaMultiplicacion( ejercicio, tama1, tama2, digLimit );
			break;
		case 'D':
		case 'd':
		case '/': 	tama2 = (byte)atoi((char*)strtok( NULL, DELIM ));
					ejercicio = generaDivision( ejercicio, tama1, tama2, digLimit );
			break;
		default: ejercicio = (char*)NULL;
	}

	free( buffer );
	return ejercicio;
}

int incluyeEjercicio( char *orden, unsigned int numEjercicio, byte digLimit ) {
	char linea[MAXLIN+1];
	char ejercicio[MAXLIN+1];

	parseaOrden( orden, ejercicio, digLimit );
	sprintf( linea, "ejercicio %d: \t%s", numEjercicio, ejercicio );
	return ( puts( linea ) );
}

void getToken( char* ejercicio, byte *multiplicador, byte *digLimit, int numLinea ) {
	char *cadena, *ptr, *subToken;
	int ejerTama = strlen( ejercicio );

	*multiplicador = 1;
	*digLimit = 0;

	if ( !(cadena = calloc( ejerTama, sizeof( char ) ) ) ) {
		gERRNO = errno;
		raiseError( gERRNO, 0 );
	}

	if ( !(subToken = calloc( ejerTama, sizeof( char ) ) ) ) {
		gERRNO = errno;
		raiseError( gERRNO, 0 );
	}

	strncpy( cadena, ejercicio, ejerTama ); //sprintf( cadena, "%s", ejercicio );
	if ( (ptr = strrchr( cadena, OP_DELIM )) ) *ptr= '\0';
	ptr = strtok( cadena, TOK_DELIM );
	while ( (ptr = strtok( NULL, TOK_DELIM )) ) {
		switch( *ptr ) {
		case 'M':
		case 'm': 	sprintf( subToken, "%s", ++ptr );
					*multiplicador = (byte)atoi( ptr );
					break;
		case 'L':
		case 'l': 	sprintf( subToken, "%s", ++ptr );
					*digLimit = (byte)atoi( ptr );
					break;
		default:
					gERRNO = -1;
					raiseError( gERRNO, numLinea );
					break;
		}
	}
	free( subToken );
	free( cadena );
}

void generaCuadernillo( char* argv[] ) {
	FILE *fp;
	fpos_t fInicio;
	char *ejercicio;
	unsigned int numEjercicio = 0;
	size_t tamanio = 1;
	byte multiplicador;
	char currCar;
	byte item = 0;
	char linea[MAXLIN+1];
	boolean saltaLinea = FALSE;
	byte digLimit;
	int numLinea = 1;


	if ( !( fp = fopen( argv[1], "r" ) ) ) {
		gERRNO = errno;
		raiseError( gERRNO, 0 );
	}

	strncpy( linea, "EJERCICIOS DE MATEMÁTICAS", MAXLIN ); puts( centra( linea, MAXLIN ) );
	strncpy( linea, "-------------------------", MAXLIN ); puts( centra( linea, MAXLIN ) );
	puts("");

	fgetpos( fp, &fInicio );

	while ( !feof( fp ) ) {
		currCar = (char)fgetc( fp ); ++tamanio;
		if ( currCar == COMMENT ) saltaLinea = TRUE;
		if ( currCar == OP_DELIM || currCar == COMMENT ) {
			if ( !( ejercicio = (char*)calloc( tamanio, sizeof( char ) ) ) ) {
				gERRNO = errno;
				raiseError( gERRNO, 0 );
			}
			fsetpos( fp, &fInicio );
			fread( (char*)ejercicio, sizeof( char ), tamanio-1, fp );
			if ( !saltaLinea ) {
				getToken( ejercicio, (byte*)&multiplicador, (byte*)&digLimit, numLinea );
				for( item = 1; item <= multiplicador; ++item) incluyeEjercicio( ejercicio, ++numEjercicio, digLimit ) ;
			}
			if ( currCar == COMMENT ) {
				while ( currCar != '\n' ) currCar = (char)fgetc( fp );
				saltaLinea = FALSE;
			}
			tamanio = 1;
			fgetpos( fp, &fInicio );
			free( ejercicio );
		}
		++numLinea;
	}
	fclose( fp );
}

int main( int argc, char* argv[] ) {
	if ( argc != 2 ) {
		usage();
	} else {
		switch ( *argv[1] ){
			case '-':
				if (*(argv[1]+1) == '?') usage();
				else {
					puts( "Opción no reconocida.");
					exit( 0 );
				}
				break;
			default:
				randomize();
				generaCuadernillo( argv );
		}
	}
	return 0;
}
