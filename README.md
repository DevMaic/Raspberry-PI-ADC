<h1>
  <p align="center" width="100%">
    <img width="30%" src="https://softex.br/wp-content/uploads/2024/09/EmbarcaTech_logo_Azul-1030x428.png">
  </p>
</h1>

# ✨Tecnologias
Esse projeto foi desenvolvido com as seguintes tecnologias.
- Placa Raspberry Pi Pico W
- Raspberry Pi Pico SDK
- WokWi
- C/C++

# 💻Projeto
Projeto Desenvolvido durante o programa de capacitação profissional em microcontrolados e sistemas embarcados para estudantes de nível superior, polo Juazeiro-BA, na Universidade Federal do Vale do São Francisco (UNIVASF).

# 🚀Como rodar
### **Softwares Necessários**
1. **VS Code** com a extensão **Raspberry Pi Pico** instalada.
2. **CMake** e **Ninja** configurados.
3. **SDK do Raspberry Pi Pico** corretamente configurado.

### **Clonando o Repositório**
Para começar, clone o repositório no seu computador:
```bash
git clone https://github.com/DevMaic/Raspberry-PI-ADC
cd Raspberry-PI-ADC
```
---


### **Execução na Placa BitDogLab**
#### **1. Coloque em Modo Reboot**
1. Aperte o botão **BOOTSEL** no microcontrolador Raspberry Pi Pico W.
2. Ao mesmo tempo, aperte o botão de **Reset**.
#### **2. Upload de Arquivo `DisplayC.uf2`**
1. Abra a pasta `build` incluída no repositório.
2. Mova o arquivo `DisplayC.uf2` para a placa de desenvolvimento.
#### **3. Acompanhar Execução do Programa**
1. Mova o joystick e veja-os o cursor se mover no **display SSD1306**.
2. Mova o joystick e veja-os os leds azul e vermelho mudarem de intensidade.
3. Pressione **Botão A** para desligar o PWM dos leds.
4. Pressione **Botão dp joystick** para çigar ou desligar o led verde.
   
---
