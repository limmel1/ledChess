# chessboard GUI

import PySimpleGUI as sg

layout = [[sg.Text("Test GUI, click 'Quit' to quit")], [sg.Button("Quit")]]

# GUI window
window = sg.Window("Demo", layout, margins=(100,100))

# event loop
while True:
    event, values = window.read()
    # end program if window is closed or "Quit" is pressed
    if event == "Quit" or event == sg.WIN_CLOSED:
        break

window.close()