import os
import subprocess

def format_files(directory):
    for filename in os.listdir(directory):
        if filename.endswith('.cpp') or filename.endswith('.h'):
            file_path = os.path.join(directory, filename)
            print(f'Formatting {file_path}')
            subprocess.run(['clang-format', '-i', '-style={IndentWidth: 4}', file_path])

            
format_files('C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS')