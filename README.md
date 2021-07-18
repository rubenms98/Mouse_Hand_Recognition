# Mouse_Hand_Recognition
Code that creates, compilates and exports a neural network readable for Arduino with which we can control in a simple way the mouse of our computer.

In the folder "1_Red_Neuronal_Entrenamiento" we have two folders, the one for the data generator of the arm and the one for the hand.
	To obtain the data from one of the two, what we do is run the corresponding Arduino and Python programs at the same time.
	program at the same time. At the end it will generate a text file. We will extract it to the folder
	previous folder, "1_Red_Neuronal_Entrenamiento". We will do this process with both hand and fingers. Then we will run the
	program "Tratamiento_Datos_Input_NNN.py" which will generate the final file which is the one we will later use to train the neural network. 
	we will use to train the neural network.


In the folder "2_Models" we find "Red_Y_Conversion_Final", both .py and .ipynb, which is the same program but in Python or Jupy.
	in Python or Jupyter Notebooks respectively. In it is the network created, trained, exported and optimized for Arduino.
	for Arduino. With this program, we download the Netron models, which are the .h5 file and the two
	.tflite. To view them you need to download Netron https://netron.app/. We also download
	two .cc files which are the character arrays (optimised and unoptimised) to be subsequently interpreted by the Arduino.
	interpreted by Arduino.


In the folder "3_Histogramas_Pesos" we have the data of the weights of the hidden layers of the network, downloaded with 
	Netron, and a small program that represents and compares them quantized and unquantized.


In the folder "4_Arduino" we have the programs in which is the neural network, which will be executed in the Nano 33 BLE, 
	and the one that receives the data via BLE, which will run on the Nano IoT. In the Arduino folder of the one sending the data 
	data "Hand_Recognition" there is a .cpp file and a .h file. In the .cpp file we will need to copy the
	character matrix and the number of characters of this matrix that we have obtained previously in a .cc file.
	.cc file. The .h file is not touched.
