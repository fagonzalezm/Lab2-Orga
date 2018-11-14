#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estructura de un registro: Lista enlazada

typedef struct registro{
	int valor;
	struct registro * siguiente;
}registro;

//TDA Lista enlazada

int contenidoValorRegistro(registro * nodo){
	if(!nodo){
		return 0;
	}
	return nodo -> valor;
}

registro * siguienteRegistro(registro * nodo){
	if(!nodo){
		return NULL;
	}
	return nodo -> siguiente;
}

registro * crearRegistro(int nuevoValor, registro * nuevoRegistro, char * nuevoNombreRegistro){
	registro * nuevo = NULL;
	nuevo =(registro *)malloc(sizeof(registro));

	nuevo -> siguiente = (registro *)malloc(sizeof(registro));

	nuevo -> valor = nuevoValor;
	nuevo -> siguiente = nuevoRegistro;

	return nuevo;
}

registro * anularRegistro(registro * raiz){
	if(raiz == NULL){
		return raiz;
	}
	raiz -> siguiente = anularRegistro(raiz -> siguiente);
	free(raiz);
	raiz = NULL;
	return raiz;
}

registro * insertarRegistro(registro * cabecera, int nuevoValor, char * nuevoNombreRegistro){
	registro * nuevo = crearRegistro(nuevoValor, NULL, nuevoNombreRegistro);

	if(cabecera == NULL){
		return nuevo;
	}

	registro * indice = cabecera;
	while(indice -> siguiente != NULL){
		indice = indice -> siguiente;
	}
	indice -> siguiente = nuevo;
	indice -> siguiente -> siguiente = NULL;
	return cabecera;
}

registro * buscarPosicionRegistro(registro * cabecera, int posicion){
	registro * indice = cabecera;
	int contador = 0;
	while(indice != NULL){
		if(contador == posicion){
			return indice;
		}
		contador = contador + 1;
		indice = indice -> siguiente;
	}
	return NULL;
}
//Funcion: Busca la etiqueta de un jump o branch para luego saltar a esa linea
//Entrada:	Areglo de cadena de caracteres, donde cada elemento es una linea de codigo; puntero a char que es la etiqueta que se busca y la cantidad de instrucciones
//Salida: El indice de la linea de instruccion que posee la etiqueta
int buscarEtiqueta(char ** instrucciones, char * etiqueta, int numeroDeInstrucciones){
	int contador = 0;
	for(contador = 0; contador < numeroDeInstrucciones; contador++){
		if(strncmp(etiqueta, instrucciones[contador], strlen(etiqueta)-1) == 0){
			return contador;
		}
	}
}

//Instrucciones: Cada instruccion es una función con su mismo nombre

int addi(int operando, int entero){
	return operando + entero; 
}
int subi(int minuendo, int entero){
	return minuendo - entero;
}
void beq(int * lineaDeEjecucion, int primerNumero, int segundoNumero, int lineaObjetivo){
	if(primerNumero == segundoNumero){
		*lineaDeEjecucion = lineaObjetivo;
	}
	else{
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
}
void jump(int * lineaDeEjecucion, int lineaDestino){
		*lineaDeEjecucion = lineaDestino;
}
int add(int operando1, int operando2){
	return operando1 + operando2;
}
int sub(int minuendo, int sustraendo){
	return minuendo - sustraendo;
}
int mul(int factor1,int factor2){
	return factor1 * factor2;
}
int divi(int dividendo, int divisor){
	if(divisor != 0){
		return dividendo/divisor;
	}
	return 0;
}
void lw(registro * destino,registro * direccion, int indice){
	registro * direccionPorIndice = buscarPosicionRegistro(direccion, indice);
	if(direccionPorIndice == NULL){
		destino -> valor = 0;
	}
	else{
		destino -> valor = contenidoValorRegistro(direccionPorIndice);
	}
}
void sw(registro * destino,registro * inicio, int indice){
	registro * direccionPorIndice = buscarPosicionRegistro(inicio, indice);
	if(direccionPorIndice == NULL){
		destino -> valor = 0;
	}
	else{
		destino -> valor = contenidoValorRegistro(direccionPorIndice);
	}
}

//Funcion: Cuenta las lineas de un archivo
//Entrada: Nombre de un archivo
int numeroDeInstrucciones(char * path){
	int respuesta = 0;
	char aux[100];
	FILE * archivo;
	archivo = fopen(path, "r");

	if(archivo == NULL){
		printf("Archivo no encontrado");
	}

	else{
		while(!feof(archivo)){
			fgets(aux,50,archivo);
			respuesta ++;
		}
	}
	fclose(archivo);
	return respuesta;
}

//Funcion: Lee el primer archivo de entrada y lo guarda en un arreglo en donde cada linea es un elemento del arreglo
//Entrada: Nombre del archivo
char ** leerArchivo1(char * path, char ** instrucciones){
	char * aux = NULL;
	FILE * archivo;
	archivo = fopen(path, "r");
	int contador = 0;

	if(archivo == NULL){
		printf("Archivo no encontrado");
	}

	else{
		while(!feof(archivo)){
			fgets(instrucciones[contador], 50, archivo);
			contador ++;
		}
	}
	return instrucciones;
}

//Funcion: Lee el segundo archivo de entrada y actualiza el estado stuck at 0 de las lineas de control
//Entrada: Cinco enteros que representan a las lineas de control y el nombre del archivo
//Nota: El estado 0 de la lineas de control representa que existe stuck at 0
void leerArchivo2(int * jump, int * branch, int * regwrite, int * memread, int * memwrite, char * path){
	//Indice de lectura
	char * aux = NULL;
	aux = (char *)malloc(sizeof(char) * 50);
	FILE * archivo;
	archivo = fopen(path, "r");

	if(archivo == NULL){
		printf("Archivo no encontrado");
	}

	else{
		while(!feof(archivo)){
			fscanf(archivo, "%s", aux);
			if(strcmp(aux, "Jump") == 0){
				fscanf(archivo, "%s", aux);
				if(strcmp(aux, "x") == 0){
					*jump = 1;
				}
			}
			if(strcmp(aux, "Branch") == 0){
				fscanf(archivo, "%s", aux);
				if(strcmp(aux, "x") == 0){
					*branch = 1;
				}

			} 
			if(strcmp(aux, "Regwrite") == 0){
				fscanf(archivo, "%s", aux);
				if(strcmp(aux, "x") == 0){
					*regwrite = 1;
				}

			} 
			if(strcmp(aux, "Memread") == 0){
				fscanf(archivo, "%s", aux);
				if(strcmp(aux, "x") == 0){
					*memread = 1;
				}

			} 
			if(strcmp(aux, "Memwrite") == 0){
				fscanf(archivo, "%s", aux);
				if(strcmp(aux, "x") == 0){
					*memwrite = 1;
				}

			} 

		}
	}
	fclose(archivo);
	free(aux);
	aux = NULL;
}

//Funcion: Escribe en el archivo de salida2 la linea que se está leyendo
//Entrada: Puntero a char con la linea leida y puntero a FILE
void escribirLinea(char * lineaLeida, FILE * archivo1){
	archivo1 = fopen("salida1.txt", "a");
	fprintf(archivo1, "%s" ,lineaLeida);
	fclose(archivo1);
}

//Funcion: Escribe en el archivo de salida1 los valores de los registros
//Entrada: Puntero a FILE y untero a char con la linea leida
void escribirRegistros(FILE * archivo2, registro * zero, registro * at, registro * v0, registro * v1, registro * a0, registro * a1, registro * a2, registro * a3, registro * t0, registro * t1, registro * t2, registro * t3, registro * t4, registro * t5, registro * t6, registro * t7, registro * s0, registro * s1, registro * s2, registro * s3, registro * s4, registro * s5, registro * s6, registro * s7, registro * t8, registro * t9, registro * k0, registro * k1, registro * gp, registro * sp, registro * fp, registro * ra){
	archivo2 = fopen("salida2.csv", "a");
	//Los if son para que quede mas legible el archivo de salida
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(zero));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(at));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(v0));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(v1));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(a0));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(a1));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(a2));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(a3));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t0));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t1));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t2));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t3));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t4));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t5));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t6));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t7));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s0));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s1));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s2));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s3));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s4));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s5));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s6));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(s7));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t8));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(t9));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(k0));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(k1));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(gp));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(sp));
	fprintf(archivo2,"%d\t" , contenidoValorRegistro(fp));
	fprintf(archivo2,"%d\n" , contenidoValorRegistro(ra));
	fclose(archivo2);
}

//Funcion: Identifica el registro que indica un char
//Entrada: El char con el registro y los registros
//Salida: Cabecera del puntero al registro que indica el char
registro * identificarRegistro (char * registroEntrada, registro * zero, registro * at, registro * v0, registro * v1, registro * a0, registro * a1, registro * a2, registro * a3, registro * t0, registro * t1, registro * t2, registro * t3, registro * t4, registro * t5, registro * t6, registro * t7, registro * s0, registro * s1, registro * s2, registro * s3, registro * s4, registro * s5, registro * s6, registro * s7, registro * t8, registro * t9, registro * k0, registro * k1, registro * gp, registro * sp, registro * fp, registro * ra){
	if(strcmp(registroEntrada,"$zero") == 0){
		return zero;
	}
	if(strcmp(registroEntrada,"$at") == 0){
		return at;
	}
	if(strcmp(registroEntrada,"$v0") == 0){
		return v0;
	}
	if(strcmp(registroEntrada,"$v1") == 0){
		return v1;
	}
	if(strcmp(registroEntrada,"$a0") == 0){
		return a0;
	}
	if(strcmp(registroEntrada,"$a1") == 0){
		return a1;
	}
	if(strcmp(registroEntrada,"$a2") == 0){
		return a2;
	}
	if(strcmp(registroEntrada,"$a3") == 0){
		return a3;
	}
	if(strcmp(registroEntrada,"$t0") == 0){
		return t0;
	}
	if(strcmp(registroEntrada,"$t1") == 0){
		return t1;
	}
	if(strcmp(registroEntrada,"$t2") == 0){
		return t2;
	}
	if(strcmp(registroEntrada,"$t3") == 0){
		return t3;
	}
	if(strcmp(registroEntrada,"$t4") == 0){
		return t4;
	}
	if(strcmp(registroEntrada,"$t5") == 0){
		return t5;
	}
	if(strcmp(registroEntrada,"$t6") == 0){
		return t6;
	}
	if(strcmp(registroEntrada,"$t7") == 0){
		return t7;
	}
	if(strcmp(registroEntrada,"$s0") == 0){
		return s0;
	}
	if(strcmp(registroEntrada,"$s1") == 0){
		return s1;
	}
	if(strcmp(registroEntrada,"$s2") == 0){
		return s2;
	}
	if(strcmp(registroEntrada,"$s3") == 0){
		return s3;
	}
	if(strcmp(registroEntrada,"$s4") == 0){
		return s4;
	}
	if(strcmp(registroEntrada,"$s5") == 0){
		return s5;
	}
	if(strcmp(registroEntrada,"$s6") == 0){
		return s6;
	}
	if(strcmp(registroEntrada,"$s7") == 0){
		return s7;
	}
	if(strcmp(registroEntrada,"$t8") == 0){
		return t8;
	}
	if(strcmp(registroEntrada,"$t9") == 0){
		return t9;
	}
	if(strcmp(registroEntrada,"$k0") == 0){
		return k0;
	}
	if(strcmp(registroEntrada,"$k1") == 0){
		return k1;
	}
	if(strcmp(registroEntrada,"$gp") == 0){
		return gp;
	}
	if(strcmp(registroEntrada,"$sp") == 0){
		return sp;
	}
	if(strcmp(registroEntrada,"$fp") == 0){
		return fp;
	}
	if(strcmp(registroEntrada,"$ra") == 0){
		return ra;
	}
	return NULL;
}


//Funcion: Ejecuta una instruccion dependiendo de las entradas
//Entrada: punteros a FILE de las dos salidas,tres cadenas de caracteres, los punteros a cada registro y las lineas de control
void ejecutarInstruccion(FILE * archivo1, FILE * archivo2, char * primeraPalabra, char * segundaPalabra, char * terceraPalabra, char * cuartaPalabra, char ** instrucciones, int * lineaDeEjecucion, int cantidadDeLineas, registro * zero, registro * at, registro * v0, registro * v1, registro * a0, registro * a1, registro * a2, registro * a3, registro * t0, registro * t1, registro * t2, registro * t3, registro * t4, registro * t5, registro * t6, registro * t7, registro * s0, registro * s1, registro * s2, registro * s3, registro * s4, registro * s5, registro * s6, registro * s7, registro * t8, registro * t9, registro * k0, registro * k1, registro * gp, registro * sp, registro * fp, registro * ra, int * jump_, int * branch, int * regwrite, int * memread, int * memwrite){
	if(strcmp(primeraPalabra,"addi") == 0)
	{	
		registro * resultado = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int operando = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int constante = atoi(cuartaPalabra);
		int total = addi(operando, constante);
		if(*regwrite == 1){
			resultado -> valor = total;
		} 
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;

	}
	else if(strcmp(primeraPalabra,"subi") == 0)
	{
		registro * resultado = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int minuendo = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int constante = atoi(cuartaPalabra);
		int total = subi(minuendo, constante);
		if(*regwrite == 1){
			resultado -> valor = total;
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else if(strcmp(primeraPalabra,"jump") == 0)
	{
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int lineaDestino = *lineaDeEjecucion + 1;
		if(*jump_ == 1){
			lineaDestino = buscarEtiqueta(instrucciones, segundaPalabra, cantidadDeLineas);
		}
		jump(lineaDeEjecucion,lineaDestino);
	}
	else if(strcmp(primeraPalabra,"beq") == 0)
	{
		int primerNumero = contenidoValorRegistro(identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int segundoNumero = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int lineaDestino = buscarEtiqueta(instrucciones, cuartaPalabra, cantidadDeLineas);
		if(*branch == 1){
			beq(lineaDeEjecucion,primerNumero, segundoNumero, lineaDestino);
		}
		if(*branch == 0){
			*lineaDeEjecucion = *lineaDeEjecucion + 1;
		}
	}
	else if(strcmp(primeraPalabra,"add") == 0)
	{
		registro * resultado = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int operando1 = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int operando2 = contenidoValorRegistro(identificarRegistro(cuartaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int total = add(operando1, operando2);
		if(*regwrite == 1){
			resultado -> valor = total;
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else if(strcmp(primeraPalabra,"sub") == 0)
	{
		registro * resultado = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int minuendo = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int sustraendo = contenidoValorRegistro(identificarRegistro(cuartaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int total = sub(minuendo, sustraendo);
		if(*regwrite == 1){
			resultado -> valor = total;
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else if(strcmp(primeraPalabra,"mul") == 0)
	{
		registro * resultado = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int factor1 = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int factor2 = contenidoValorRegistro(identificarRegistro(cuartaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int total = mul(factor1, factor2);
		if(*regwrite == 1){
			resultado -> valor = total;
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else if(strcmp(primeraPalabra,"div") == 0)
	{
		registro * resultado = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int dividendo = contenidoValorRegistro(identificarRegistro(terceraPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int divisor = contenidoValorRegistro(identificarRegistro(cuartaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra));
		int total = divi(dividendo, divisor);
		if(*regwrite == 1){
			resultado -> valor = total;
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else if(strcmp(primeraPalabra,"lw") == 0)
	{	
		registro * destino = NULL;
		registro * direccion = NULL;
		if(*memread == 1){
			destino = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
			int byte = atoi(terceraPalabra);
			byte = byte / 4;
			direccion = identificarRegistro(cuartaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);

			if(*regwrite == 1){
				lw(destino, direccion, byte);
			}
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else if(strcmp(primeraPalabra,"sw") == 0)
	{
		registro * inicio = NULL;
		registro * destino = NULL;

		inicio = identificarRegistro(segundaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		int byte = atoi(terceraPalabra);
		byte = byte / 4;
		destino = identificarRegistro(cuartaPalabra, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);

		if(*memwrite == 1){
			sw(destino, inicio, byte);
		}
		escribirLinea(instrucciones[*lineaDeEjecucion], archivo1);
		escribirRegistros(archivo2, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra);
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
	else{ //Es una etiqueta
		*lineaDeEjecucion = *lineaDeEjecucion + 1;
	}
}


//Funcion: Lee una linea del codigo, luego ejecuta la instruccion de esa linea.
//Entrada: Cadena de caracteres y los punteros a cada registro y las lineas de control
void leerLinea(FILE * archivo1, FILE * archivo2, char ** instrucciones, char * linea, int *lineaDeEjecucion, int cantidadDeLineas, registro * zero, registro * at, registro * v0, registro * v1, registro * a0, registro * a1, registro * a2, registro * a3, registro * t0, registro * t1, registro * t2, registro * t3, registro * t4, registro * t5, registro * t6, registro * t7, registro * s0, registro * s1, registro * s2, registro * s3, registro * s4, registro * s5, registro * s6, registro * s7, registro * t8, registro * t9, registro * k0, registro * k1, registro * gp, registro * sp, registro * fp, registro * ra, int *jump, int * branch, int * regwrite, int * memread, int * memwrite){
	char primeraPalabra[50];
	char segundaPalabra[50];
	char terceraPalabra[50];
	char cuartaPalabra[50];
	//Separo el string por espacios, comas y saltos de linea. Se guardan en token
	char separador[] = " ,()\n\t\r";
	char * token = NULL;
	token = strtok(linea, separador); //Primera palabra
	strcpy(primeraPalabra, token);
	token = strtok(NULL, separador); //Segunda palabra
	if(token != NULL){ //Si no hay segunda palabra, no se hace
		strcpy(segundaPalabra, token);
	}
	else{
		segundaPalabra[0] = '\0';
	}
	token = strtok(NULL, separador); //Tercera palabra
	if(token != NULL){ //Si no hay tercera palabra, no se hace
		strcpy(terceraPalabra, token);
	}
	else{
		terceraPalabra[0] = '\0';
	}
	token = strtok(NULL, separador); //Cuarta palabra
	if(token != NULL){ //Si no hay cuarta palabra, no se hace
		strcpy(cuartaPalabra, token); 
	}
	else{
		cuartaPalabra[0] = '\0';
	}
	ejecutarInstruccion(archivo1, archivo2, primeraPalabra, segundaPalabra, terceraPalabra, cuartaPalabra, instrucciones, lineaDeEjecucion, cantidadDeLineas, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra, jump, branch, regwrite, memread, memwrite);
}

int main(){
	int i;
	//Instrucciones del primer archivo de entrada
	char ** instrucciones = NULL;
	//Nombre de archivos
	char * nombreArchivo1 = NULL;
	char * nombreArchivo2 = NULL;
	nombreArchivo1 = (char *)malloc(sizeof(char) * 150);
	nombreArchivo2 = (char *)malloc(sizeof(char) * 150);
	int cantidadDeLineas = 0;
	//Linea de ejecucion
	int * lineaDeEjecucion = NULL;
	lineaDeEjecucion = (int *)malloc(sizeof(int));
	*lineaDeEjecucion = 0;
	//Estado lineas de control
	int * jump = NULL;
	int * branch = NULL;
	int * regwrite = NULL;
	int * memread = NULL;
	int * memwrite = NULL;
	jump = (int *)malloc(sizeof(int));
	branch = (int *)malloc(sizeof(int));
	regwrite = (int *)malloc(sizeof(int));
	memread = (int *)malloc(sizeof(int));
	memwrite = (int *)malloc(sizeof(int));
	*jump = 0;
	*branch = 0;
	*regwrite = 0;
	*memread = 0;
	*memwrite = 0;
	//Estado registros
	registro * zero = insertarRegistro(NULL,0,"zero");
	registro * at = insertarRegistro(NULL,0,"at");
	registro * v0 = insertarRegistro(NULL,0,"v0");
	registro * v1 = insertarRegistro(NULL,0,"v1");
	registro * a0 = insertarRegistro(NULL,0,"a0");
	registro * a1 = insertarRegistro(NULL,0,"a1");
	registro * a2 = insertarRegistro(NULL,0,"a2");
	registro * a3 = insertarRegistro(NULL,0,"a3");
	registro * t0 = insertarRegistro(NULL,0,"t0");
	registro * t1 = insertarRegistro(NULL,0,"t1");
	registro * t2 = insertarRegistro(NULL,0,"t2");
	registro * t3 = insertarRegistro(NULL,0,"t3");
	registro * t4 = insertarRegistro(NULL,0,"t4");
	registro * t5 = insertarRegistro(NULL,0,"t5");
	registro * t6 = insertarRegistro(NULL,0,"t6");
	registro * t7 = insertarRegistro(NULL,0,"t7");
	registro * s0 = insertarRegistro(NULL,0,"s0");
	registro * s1 = insertarRegistro(NULL,0,"s1");
	registro * s2 = insertarRegistro(NULL,0,"s2");
	registro * s3 = insertarRegistro(NULL,0,"s3");
	registro * s4 = insertarRegistro(NULL,0,"s4");
	registro * s5 = insertarRegistro(NULL,0,"s5");
	registro * s6 = insertarRegistro(NULL,0,"s6");
	registro * s7 = insertarRegistro(NULL,0,"s7");
	registro * t8 = insertarRegistro(NULL,0,"t8");
	registro * t9 = insertarRegistro(NULL,0,"t9");
	registro * k0 = insertarRegistro(NULL,0,"k0");
	registro * k1 = insertarRegistro(NULL,0,"k1");
	registro * gp = insertarRegistro(NULL,0,"gp");
	registro * sp = insertarRegistro(NULL,0,"sp");
	registro * fp = insertarRegistro(NULL,0,"fp");
	registro * ra = insertarRegistro(NULL,0,"ra");
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("\n  Nombre del primer archivo: ");
    scanf("%s", nombreArchivo1);
    cantidadDeLineas = numeroDeInstrucciones(nombreArchivo1);
    instrucciones = (char **)malloc(sizeof(char*) * cantidadDeLineas);
    for(i=0; i < cantidadDeLineas; i++){
    	instrucciones[i] = (char *)malloc(sizeof(char) * 50);
    }
    instrucciones = leerArchivo1(nombreArchivo1, instrucciones);
    printf("   OK!\n\n");
    printf("   Instrcciones en MIPS:\n");
    for(i=0; i < cantidadDeLineas; i++){
    	printf("   %s", instrucciones[i]);
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("\n   Nombre del segundo archivo: ");
    scanf("%s", nombreArchivo2);
    leerArchivo2(jump, branch, regwrite, memread, memwrite, nombreArchivo2);
    printf("   OK!\n\n");
    printf("   Estado lineas de control:\n   Jump: %d\n   Branch: %d\n   Regwrite: %d\n   Memread: %d\n   Memwrite: %d\n", *jump, *branch, *regwrite, *memread, *memwrite);
///////77/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////77//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(strcmp(nombreArchivo1, "") == 0 || strcmp(nombreArchivo2, "") == 0){
    	printf("   No se han cargado los archivos de entrada\n");
    }
    else{
    	//Se crean los archivos
    	FILE * archivo1;
		archivo1 = fopen("salida1.txt", "w");
		fclose(archivo1);
		FILE * archivo2;
		archivo2 = fopen("salida2.csv", "w");
		fprintf(archivo2, "$zero\t$at\t$v0\t$v1\t$a0\t$a1\t$a2\t$a3\t$t0\t$t1\t$t2\t$t3\t$t4\t$t5\t$t6\t$t7\t$s0\t$s1\t$s2\t$s3\t$s4\t$s5\t$s6\t$s7\t$t8\t$t9\t$k0\t$k1\t$gp\t$sp\t$fp\t$ra\n");
		fclose(archivo2);
		//
		while(*lineaDeEjecucion < cantidadDeLineas){
			char linea[50];
			strcpy(linea, instrucciones[*lineaDeEjecucion]);
    		leerLinea(archivo1, archivo2, instrucciones, linea, lineaDeEjecucion, cantidadDeLineas, zero, at, v0, v1, a0, a1, a2, a3, t0, t1, t2, t3, t4, t5, t6, t7, s0, s1, s2, s3, s4, s5, s6, s7, t8, t9, k0, k1, gp, sp, fp, ra, jump, branch, regwrite, memread, memwrite);
    	}					
	}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Liberar memoria nombre de archivos
    free(nombreArchivo1);
    free(nombreArchivo2);
    nombreArchivo1 = NULL;
    nombreArchivo2 = NULL;
    //Liberar memoria linea de ejecucion
    free(lineaDeEjecucion);
    lineaDeEjecucion = NULL;
    //Liberar memoria lineas de control
    free(jump);
    free(branch);
    free(regwrite);
    free(memread);
    free(memwrite);
    jump = NULL;
    branch = NULL;
    regwrite = NULL;
    memread = NULL;
    memwrite = NULL;
    //Liberar memoria instrucciones
    if(instrucciones != NULL){
    	for(i=0; i < cantidadDeLineas; i++){
    		free(instrucciones[i]);
    		instrucciones[i] = NULL;
    	}
    	free(instrucciones);
    	instrucciones = NULL;
    }
    //Liberar memoria registros
    anularRegistro(zero);
    anularRegistro(at);
    anularRegistro(v0);
    anularRegistro(v1);
    anularRegistro(a0);
    anularRegistro(a1);
    anularRegistro(a2);
    anularRegistro(a3);
    anularRegistro(t0);
    anularRegistro(t1);
    anularRegistro(t2);
    anularRegistro(t3);
    anularRegistro(t4);
    anularRegistro(t5);
    anularRegistro(t6);
    anularRegistro(t7);
    anularRegistro(s0);
    anularRegistro(s1);
    anularRegistro(s2);
    anularRegistro(s3);
    anularRegistro(s4);
    anularRegistro(s5);
    anularRegistro(s6);
    anularRegistro(s7);
    anularRegistro(t8);
    anularRegistro(t9);
    anularRegistro(k0);
    anularRegistro(k1);
    anularRegistro(gp);
    anularRegistro(sp);
    anularRegistro(fp);
    anularRegistro(ra);
    return 0;
}