# -*- coding: utf-8 -*-
"""
Created on Sat May 29 20:16:44 2021

@author: HOME
"""

import numpy as np
import serial
import mouse
import keyboard

arduinoPort = "COM5"
baud = 115200 
baud_UNO = 9600
timeOut = 1.0

arduino = serial.Serial(arduinoPort, baud, timeout = timeOut)

linea = 0

def vec_to_binary (vec_input):
    
    output_vector = np.zeros(len(vec_input))
    maxpos = vec_input.index(max(vec_input))
    
    for i in range(len(vec_input)):
        if i != maxpos:
            output_vector[i] = 0
        else:
            output_vector[i] = 1
            
    return output_vector 


valor1 = valor2 = valor2_scroll = 0
contador_right = contador_left = contador_scroll = 0
                        
while(not(keyboard.is_pressed('c'))):
    
    line_arduino = arduino.readline()
    line_decoded = line_arduino.decode("utf-8").split(',')   
    
    print(line_decoded)

    
    vec_brazo_vertical = (float(line_decoded[1]), float(line_decoded[0]), float(line_decoded[2]))
    vec_brazo_horizontal = (float(line_decoded[4]), 0.5, float(line_decoded[3]))
    vec_dedos = (float(line_decoded[5]), float(line_decoded[6]), float(line_decoded[7]), float(line_decoded[8]))
    
    vec_brazo_horizontal_binario = vec_to_binary(vec_brazo_horizontal)
    vec_brazo_vertical_binario = vec_to_binary(vec_brazo_vertical)
    if vec_brazo_horizontal_binario[1] == 0:
        vec_brazo_vertical_binario = [0, 1, 0]
    vec_dedos_binario = vec_to_binary(vec_dedos)
    
    
    
    x = - 1 * vec_brazo_horizontal_binario[0] + 0 * vec_brazo_horizontal_binario[1] + 1 * vec_brazo_horizontal_binario[2]
    y = 1 * vec_brazo_vertical_binario[0] + 0 * vec_brazo_vertical_binario[1] - 1 * vec_brazo_vertical_binario[2]
    valor1 = valor1 + int(x * 20)
    valor2 = valor2 + int(y * 20)
    valor2_scroll = valor2_scroll + int(y * 2)

    
    mouse.move(valor1 + int(1365/2), - valor2 + int(767/2))#permite mayor velocidad de desplazamiento por pasos que pyautogui


    
    dedos = -2 * vec_dedos_binario[0] + -1 * vec_dedos_binario[1] + 1 * vec_dedos_binario[2] + 2 * vec_dedos_binario[3] 
    print(dedos)
    
    if(dedos == -2):
        contador_right = contador_left = contador_scroll = 0
    
    if (dedos == -1):
        if(contador_left == 0):
           # print("click izquierdo")
            mouse.click("left")
        contador_left = contador_left + 1
        contador_right = contador_scroll = 0
    
    if (dedos == 1):
        if(contador_right == 0):
           # print("click derecho"
            mouse.click("right")
        contador_right = contador_right + 1
        contador_left = contador_scroll = 0
        
    if (dedos == 2):
        if(contador_scroll == 0):
            mouse.wheel(- valor2_scroll)
        contador_scroll = contador_scroll + 1
        contador_left = contador_right = 0
        
    
    
    linea = linea + 1
    

print("Salimos del bucle")
arduino.close()