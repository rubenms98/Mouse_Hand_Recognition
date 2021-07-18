# -*- coding: utf-8 -*-
"""
Created on Tue May 11 11:24:14 2021

@author: HOME
"""

import numpy as np
import matplotlib.pyplot as plt
import time
import serial

arduinoPort = "COM8"
baud = 115200 
baud_UNO = 9600
timeOut = 1.0

print("\n PROGRAMA DE TOMA DE DATOS DE LAS POSICIONES DE LOS DEDOS \n")
print("\n QUE QUIERES HACER: \n")
print(" - Continuar con el programa (pulsar c) \n")
print(" - Salir del programa (pulsar cualquier otra tecla) ")

entrada = input()
N_Dedos = 4

if entrada != 'c':
    
    arduino = serial.Serial(arduinoPort, baud, timeout = timeOut)
    arduino.write(str(entrada).encode())
    arduino.close()
    print("\n EL PROGRAMA HA FINALIZADO \n")
    
else:
    
    print("\n CONTINUAMOS CON EL PROGRAMA \n")
    arduino = serial.Serial(arduinoPort, baud, timeout = timeOut)
    arduino.write(str(entrada).encode())
    
    print(" A continuacion, el programa va a guardar cuatro posiciones de la mano:\n")
    print("Posición de reposo, click derecho, click izquierdo y coger/arrastar.\n")
    N_Posiciones = 4
    
    print("\n A continuacion, introducir las lineas que quieres por posicion")
    Lineas_Posicion = input()
    
    print("\n AHORRA VAMOS A ELEGIR CADA UNA DE LAS POSICIONES Y ETIQUETARLAS")
    print("\n Pulsa una tecla para cada posicion \n")
    arduino.write(str(N_Posiciones).encode())
    arduino.close()
    
    i = j = tiempo = 0
    linea = 0 
    x_float = np.zeros((int(N_Dedos) * int(Lineas_Posicion), N_Dedos + N_Posiciones))
    
    while i < int(N_Posiciones):
        
        A = B = C = D = 0
        print(" - Resposo (A) \n")
        print(" - Click derecho (B) \n")
        print(" - Click izquierdo (C) \n")
        print(" - Coger/arrastrar (D) \n")
        
        posicion = input()
        
        if posicion == 'A':
            A = 1
            B = C = D = 0
        elif posicion == 'B':
            B = 1           
            A = C = D = 0
        elif posicion == 'C':            
            C = 1            
            A = B = D = 0
        elif posicion == 'D':            
            D = 1            
            A = B = C = 0
            
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
           # print(x)     
            x_float[linea] = (float(x[0]), float(x[1]), float(x[2]), float(x[3]), int(A), int(B), int(C), int(D))      
            j = j + 1    
            linea = linea + 1
            
        i = i + 1
        
        arduino.close()
        print('\n')
        
    arduino.close()

    np.savetxt("Datos_Dedos_Input_NN.txt", x_float, fmt = '%f', delimiter = '   ')
    
    plt.figure()
    plt.plot(x_float[:,0])
    plt.plot(x_float[:,1])
    plt.plot(x_float[:,2])
    plt.plot(x_float[:,3])
    plt.show()


    print("\n SE ACABO \n")
        
    

