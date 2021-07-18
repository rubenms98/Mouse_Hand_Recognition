 # -*- coding: utf-8 -*-
"""
Created on Tue Mar 30 10:24:48 2021

@author: HOME
"""

import numpy as np
import matplotlib.pyplot as plt
import serial
import time

arduinoPort = "COM8"
baud = 115200 
baud_UNO = 9600
timeOut = 1.0

print("\n QUE QUIERES HACER: \n")

print(" - Continuar con el programa (pulsar c) \n")

print(" - Salir del programa (pulsar cualquier otra tecla) ")

entrada = input()

if entrada != 'c':
    
    arduino = serial.Serial(arduinoPort, baud, timeout = timeOut)
    
    arduino.write(str(entrada).encode())
    
    arduino.close()
    
    print("\n EL PROGRAMA HA FINALIZADO \n")
    
else:
    
    print("\n CONTINUAMOS CON EL PROGRAMA \n")
    
    arduino = serial.Serial(arduinoPort, baud, timeout = timeOut)
    
    arduino.write(str(entrada).encode())
    
    print(" El programa va a necesitar una serie de valores para continuar.\n")
        
    print(" A conitnuacion, vamos a guardar 5 posiciones")
    
    N_Posiciones = 5
    
    print("\n A continuacion, introducir las lineas que quieres por posicion")
    
    Lineas_Posicion = input()
    
    print("\n AHORRA VAMOS A ELEGIR CADA UNA DE LAS POSICIONES Y ETIQUETARLAS")
    
    print("\n Pulsa una tecla para cada posicion \n")
    
    arduino.write(str(N_Posiciones).encode())
    
    arduino.close()
    
    i = j = tiempo = 0
    
    linea = 0
    
    x_float = np.zeros((int(N_Posiciones) * int(Lineas_Posicion), 8))
    
    while i < int(N_Posiciones):
        
        A = B = C = D = E = 0
        
        print(" - REPOSO (A) \n")
        
        print(" - ARRIBA (B) \n")
        
        print(" - ABAJO (C) \n")
        
        print(" - DERECHA (D) \n")
        
        print(" - IZQUIERDA (E) \n")
        
        
        posicion = input()
        
        if posicion == 'A':
            A = 1
            B = C = D = E = 0
            
        elif posicion == 'B':
            B = 1           
            A = C  = D = E = 0
            
        elif posicion == 'C':            
            C = 1            
            A = B = D = E = 0
            
        elif posicion == 'D':            
            D = 1            
            A = B = c = E = 0
            
        elif posicion == 'E':            
            E = 1            
            A = B = C = D = 0
            
        arduino = serial.Serial(arduinoPort, baud, timeout = timeOut)
            
        print("\n Coloca la posicion \n")
            
        for tiempo in range(3, 0, -1):

            time.sleep(1)
        
            print(tiempo)
    
        time.sleep(1)
        
        print("\n")
            
        j = 0
            
        while j < int(Lineas_Posicion):
            
            line = arduino.readline()
    
            x = line.decode("utf-8").split(',')
            
            #print(x)
        
            x_float[linea] = (float(x[0]), float(x[1]), float(x[2]), int(A), int(B), int(C), int(D), int(E)) 
            
            j = j + 1
            
            linea = linea + 1
        
        i = i + 1
        
        arduino.close()
        
        print('\n')
        

        
    arduino.close()

    np.savetxt("Datos_Posiciones_Brazo.txt", x_float, fmt = '%f', delimiter = '   ')
    
    print("\n SE ACABO \n")
            
   