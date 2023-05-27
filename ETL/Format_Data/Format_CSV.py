import pandas as pd
import tkinter as tk
from tkinter import filedialog

# Criando janela do Tkinter
root = tk.Tk()
root.withdraw()

# Selecionando arquivo CSV
file_path = filedialog.askopenfilename()

# Carregando o arquivo CSV em um DataFrame
df = pd.read_csv(file_path, delimiter='-', skiprows=3, header=None, names=['Data', 'Oc', 'Endereço', 'Região'])

# Exportando DataFrame para um arquivo CSV limpo
df.to_csv('ocorrencias_limpo.csv', index=False, columns=['Data', 'Oc', 'Endereço', 'Região'])
