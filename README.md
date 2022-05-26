<p align="center">
    <img width="300" src="https://drive.google.com/uc?export=view&id=1qfL4MxJq_13R_kNgUIDR8ItDM3QRHH2J"/>
</p>

<h1 align="center">IEEE UFABC - RASARM Project</h1>

<h4>⌛ <b>Projeto em Andamento</b></h4>

<p align="center">
    <a href="#-descrição-do-projeto">Descrição</a> |    
    <a href="#-requisitos">Requisitos</a> |
    <a href="#-modelo-do-braço-robótico">Modelagem</a> |
    <a href="#-como-utilizar">Como utilizar</a> |
    <a href="#-repositórios">Repositórios</a> |
    <a href="#-resultados">Resultados</a> |
    <a href="#-licença">Licença</a> |
    <a href="#-autores">Autores</a> 
</p>
    
## 🤖 **Descrição do projeto**

O projeto consiste na utilização do framework ROS (*Robot Operating Systems*) para simulação e validação de um braço robótico 5-DOF, que possuíra um sistema de controle a partir de um teclado ou por um joystick. Além disso, será avaliado, através dos programas integrados ao ROS, como *Movelt+RVIZ* e *Gazebo*, o planejamento de caminhos em tarefas de manipulação de objetos e o funcionamento consistente dos links e juntas do modelo.

Outro ponto do projeto é estabelecer um vínculo entre o modelo virtual e o real, para que os movimentos requeridos sejam validados na simulação e em seguida replicados no modelo real, visando a eficiência e estabilidade em tarefas de manipulação.

## ⚙️ **Requisitos** 

Para execução dessa simulação deve-se ter instalado alguns programas específicos que permitem o funcionamento do projeto. Prioritariamente precisa-se instalar o framework ROS, no caso desse projeto utilizamos a versão ROS 1 LTS Noetic Ninjemys direcionado para o sistema operacional Ubuntu 20.04 (Focal). Apesar da recomendação de utilização dessa distribuição Linux, outros sistema são suportados, incluíndo o Windows que possui algumas abordagens de funcionamento do ROS, porém com algumas limitações de implementação como as restrições da interface gráfica para execução do programas embutidos, especificamente RVIZ e Gazebo. <br>

Após a instalação do *framework* deve-se instalar o Movelt, plataforma para tarefas robóticas integrada ao ROS, permitindo o desenvolvimento de aplicações de manipulação complexa de objetos por braços robóticos.<br>

| [<img src="https://raw.githubusercontent.com/github/explore/80688e429a7d4ef2fca1e82350fe8e3517d3494d/topics/ubuntu/ubuntu.png" alt="Ubuntu" width="100" height="70" />](http://godban.github.io/browsers-support-badges/)| [<img src="https://insights.ubuntu.com/wp-content/uploads/e203/ROS.png" alt="ROS" width="200" height="100" />](http://godban.github.io/browsers-support-badges/)| [<img src="https://raw.githubusercontent.com/fkromer/awesome-gazebo/facc6a1a651e145c94dc3ad01826fe609cbb788c/gazebo_icon.svg" alt="GAZEBO" width="120" height="75" />](http://godban.github.io/browsers-support-badges/) | [<img src="https://moveit.ros.org/assets/logo/moveit_logo-black.png" alt="Movelt" width="200" height="30" />](http://godban.github.io/browsers-support-badges/)|
| --- | --- | --- | --- |
| [Ubuntu 20.04 LTS](https://releases.ubuntu.com/20.04/) | [ROS 1 Noetic Ninjemys](https://www.ros.org/) | [GAZEBO 9](https://gazebosim.org/home) | [Movelt 1](https://moveit.ros.org/) |

## 🦾 **Modelo do braço robótico**

Em relação à etapa de modelagem do braço robótico, optou-se pela utilização da versão *open source* BCN3D-MOVEO, que possui cinco graus de liberdade e portabilidade com o microcontrolador Arduino.

A escolha desse modelo deve-se pela sua disponibilidade de informações referente a impressão 3D do braço robótico, com o compartilhamento de arquivos BOM (*bill of materials*), STL *files* para impressão da estrutura e as CAD *designs files* no github da [BCN3D Moveo](https://github.com/BCN3D/BCN3D-Moveo).

<p align="center">
    <img width="500" src="https://www.bcn3d.com/wp-content/uploads/2016/07/IMG_7106_web-1.jpg"/>
</p>

## 💻 **Como utilizar**

Os comandos de instrução para execução do projeto são apresentados logo abaixo, em considerando a utilização do sistema operacional Ubuntu.

1. **Criar um workspace:** criar uma pasta principal através do terminal com o nome desejado, inserindo nela um subpasta nomeado como src.
   ```
   $ mkdir workspace_name/src
   ```
2. Criar o espaço de trabalho catkin, para criação dos diretórios source(src), build e devel e executar o comando CMake.
   ```
   $ cd ~/worspace_name
   $ catkin_make
   ```
3. Sempre que deseja-se executar o workspace ou após a compilação inicial deve-se executar o seguinte comando para soprepor o espaço de trabalho sob o ambiente.
   ```
   $ source devel/setup.bash
   ```
4. Crie um diretório para descrição do modelo dentro da pasta src, onde nomeação da pasta é de sua escolha.
   ```
   $ cd ~/src
   workspace_name/src$ mkdir name_robot_description
   ```
5. Em seguida, baixe os repositórios do projeto apresentados no github, colocando-os na pasta criada na etapa 4.
6. Para a etapa de execução das simulações, existe comandos específicos para cada tipo de plataforma associada ao ROS:
   a. Para a avaliação da tarefa de planejamento e manipulação de objetos com braço robótico simulado, utiliza-se a plataforma RVIZ com o *plugin* Movelt pelo seguinte comando:
      ```
      workspace_name/$ roslaunch name_robot_description rviz.launch 
      ```
   b. Para simulação 3D de cenários com obstáculos e outros objetos, incluíndo a utilização de componentes físicos como iluminação, gravidade e inércia, deve-se utilizar a plataforma Gazebo pelo seguinte comando:
      ```
      workspace_name/$ roslaunch name_robot_description gazebo.launch 
      ```

## 📁 **Repositórios**
Abaixo, apresenta-se uma descrição dos repositórios desse projeto.

### **I. model_description**
Contém diretórios para construção dos elementos do braço robótico e também para a execução das simulações.
#### urdf
Contém a file URDF (Unified Robot Description File) para descrição do braço robótico 5-DOF (BCN3D-Moveo), sendo utilizado na simulação com a plataforma RVIZ com configuração do  Movelt, como também no Gazebo.
#### launch
Contém as *files* de execução das simulações na plataforma RVIZ e Gazebo.
#### meshes
Contém os modelos 3D, no formato .STL, que são utilizados para representação do modelo no ROS. Especificamente foi dividido o robô em 9 files, com denominação de links e as partes associadas a garra.
#### rviz
Contém as configuração do ambiente RVIZ, como o plugin Moveit e especificações de visualização da janela do simulador. 

## 🏁 **Resultados**

## 📑 **Licença**

## ✅ **Autores**

<table>
    <tr>
        <td align="center"><a href="https://github.com/giovannirdias"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/99917909?v=4" width="100px;" alt=""/><br /><sub><b> Giovanni</b><br /><a> 🤖 💻 </a></td>
        <td align="center"><a href="https://github.com/marcos3939"><img style="border-radius: 50%;" src="https://avatars.githubusercontent.com/u/89995021?v=4" width="100px;" alt=""/><br /><sub><b> Marcos Pacheco</b><br /><a> 🤖 💻 </a></td>   
    </tr>            
</table>
            
