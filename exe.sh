#!/bin/bash

function mostrarCodigo() 
{
	more piSO.c
}

function compilar() 
{	
	gcc piSO.c
}

function ejecutar() 
{
	num2=2
	while test $num2 -eq 2
	do
		echo -e "\n"
		read -p "¿Cuantos pinches quieres en la cocina?: " num

		if test $num -le 0
		then
			echo -e "El numero de pinches debe ser mayor que 0"
		else
			num2=0
		fi
	done
	echo -e "\n\n RESTAURANTE CON $num PINCHES"
	./a.out $num
}

##############################################################
##                  MENU CON OPCIONES                       ##
##############################################################

while true
do
	echo  -e "---------------------------------"
	echo  -e "|       MENU RESTAURANTE        |"
	echo  -e "---------------------------------\n"
	echo  -e "\t1.- Mostrar Código"
	echo  -e "\t2.- Compilar Código"
	echo  -e "\t3.- Ejecutar Código"
	echo  -e "\t4.- Salir del Menú\n"
	read -p "Selecciona la acción que deseas realizar: " option

	case $option in
		1) echo -e "\nMostrando Código\n";mostrarCodigo;;
		2) echo -e "\nCompilando Código\n";compilar;;
		3) echo -e "\nEjecutando Código\n";ejecutar;;
		4) echo  -e "Has seleccionado salir\n";exit 0;;
		5) subl piSO.c;;
		6) subl exe.sh;;
		*) echo -e "Opcion Incorrecta"
	esac
done