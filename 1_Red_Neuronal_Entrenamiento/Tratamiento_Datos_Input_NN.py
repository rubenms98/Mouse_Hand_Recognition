# -*- coding: utf-8 -*-
"""
Created on Wed May 19 16:24:34 2021

@author: HOME
"""

import numpy as np
import matplotlib.pyplot as plt

brazo = np.loadtxt("Datos_Posiciones_Brazo.txt")
dedos = np.loadtxt("Datos_Dedos_Input_NN.txt")

N_Brazo_Inputs = 3
N_Brazo_Outputs = 5

brazo_input = brazo[:, 0 : N_Brazo_Inputs]
brazo_output = brazo[:, N_Brazo_Inputs : N_Brazo_Inputs + N_Brazo_Outputs]

N_Dedos_Inputs = 4
N_Dedos_Outputs = 4

dedos_input = dedos[:, 0 : N_Dedos_Inputs]
dedos_output = dedos[:, N_Dedos_Inputs : N_Dedos_Inputs + N_Dedos_Outputs]

np.random.shuffle(brazo)
np.random.shuffle(dedos)

filas = min(brazo.shape[0], dedos.shape[0])

complete_data = np.zeros((int(filas), int(brazo.shape[1]) + int(dedos.shape[1])))

for i in range(filas):
    complete_data[i] = np.concatenate([brazo_input[i], dedos_input[i], brazo_output[i], dedos_output[i]])
    
np.savetxt("Input_Final_NN.txt", complete_data, fmt = '%f', delimiter = '   ')
