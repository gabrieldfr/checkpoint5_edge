# Como realizar a instalação de um servidor local, recebendo e enviando informações através dele
## Grupo:

- Gabriel Rocha - RM: 550788
- Gustavo Calce - RM: 550638
- Julia Neves - RM: 98680
- Mateus Iago - RM: 550270
- Matheus Gusmão - RM: 550826
# Teoria
## Entendendo o funcionamento:
- Como funciona a interação com o servidor?
![modelo_cliente_servidor](https://github.com/gabrieldfr/sprint3_edge/assets/127216244/faeaedec-75c2-4f1a-a0c9-98485ff4a814)
Analizando a imagem podemos notar a presença de 3 elementos: publishers, broker e subscibers.

- Publishers: Serão responsáveis por enviar dados ao servidor por meio de tópicos. Exemplo: um Esp32 pode estar fazendo a medição da luminosidade do ambiente e enviando esse valor para um servidor.
  
- Broker: Um broker é um componente fundamental em sistemas de mensagens, especialmente em arquiteturas de publicação e subscrição (pub-sub). Ele atua como intermediário entre os publishers e os subscribers. O broker recebe mensagens dos publishers e as encaminha para os subscribers apropriados com base em tópicos ou regras de subscrição.
  
- Subscribers: Os subscriber irão solicitar para o broker as informações publicadas por meio dos tópicos pelos publishers
## Qual a ferramenta que será utilizada?
### Fiware
Fiware é uma plataforma de código aberto desenvolvida para apoiar o desenvolvimento de aplicações e soluções de Internet das Coisas (IoT) e cidades inteligentes. Ele fornece uma série de componentes e ferramentas que ajudam na coleta, gerenciamento, análise e compartilhamento de dados em tempo real de sensores e dispositivos IoT. Além disso, ele é responsável por simplificar o processo de instanciação dos principais GEs (Generic Enables)
## Generic Enables
### O que são?
Os Generic Enablers oferecem funcionalidades específicas que podem ser incorporadas em aplicações Fiware para facilitar o desenvolvimento de soluções IoT e cidades inteligentes. Cada Generic Enabler é projetado para tratar de um aspecto particular da criação de aplicações inteligentes, como a coleta de dados de sensores, o gerenciamento de contextos, a segurança, a análise de dados, entre outros.

### Generic Enables que iremos utilizar:
- IoT Agent MQTT: Respondável por facilitar a integração de dispositivos IoT baseados no protocolo de comunicação MQTT. Dessa forma, ele consegue traduzir a informação passada pelos dispositivos MQTT para o Orion Context Broker
- Orion Context Broker: O principal objetivo do Orion Context Broker é facilitar o acesso a informações atualizadas e em tempo real sobre objetos e entidades em um ambiente conectado.
- STH-Comet: Ele trabalha em conjunto com o Orion Context Broker, desempenhando um papel importante na coleta, armazenamento e recuperação de dados históricos de dispositivos IoT e outros tipos de sensores em tempo real.
## Arquitetura básica para projetos IoT
![arquitetura_basica_para_projetos_iot](https://github.com/gabrieldfr/sprint3_edge/assets/127216244/c8be9842-10b4-4329-80a2-3d8aed08cb77)
- **Camada de aplicação:** É onde está localizado o front-end da aplicação e as ferramentas que irão consumir dados e interagir com os dispositivos de Iot. No final, serão disponibilizados informações geradas por essas ferramentas para os consumidores/provedores de contexto. Exemplos de ferramentas presentes nessa camada: algoritmos de machine learning, inteligência artificial, analytics, dashboards, entre outros.
- **Camada de back-end:** Onde estão localizados: o Orion Context Broker, o STH-Comet e o IoT Agent MQTT. Além disso, é possível incluir os demais Generic Enables disponibilizados pela Fiware Foundation.
- **Camada de Iot:** Onde estão localizados os dispositos de IoT que estabelecem uma comunicação com o servidor através dos protocolos MQTT ou HTTP/NGSIv2.
# Prática
## Requisitos de software:
- Docker (<a href="https://docs.docker.com/desktop/install/windows-install/">Clique aqui<a/> para conferir o passo a passo de como instalar o docker)
- Postman (<a href="https://www.postman.com/downloads/">Clique aqui<a/> para conferir o site ofical para realizar a instalação do Postman)
## Instalação e Inicialização:
1. Abra o cmd e digite esse comando para clonar o repositório no github contendo o Fiware: https://github.com/fabiocabrini/fiware
2. Digite cd fiware para navegar até a pasta que se encontra o repositório
3. Digite docker compose up -d para inicializarmos os containers presentes em nossa pasta
## Realizando o Health Check no Postman:
### 1. Collection do Postman:
Essa collection irá ajudá-lo a interagir com os componetes Orion Context Broker, IoT Agent MQTT e STH-Comet, presentes na arquitetura do Fiware. Dessa forma, será possível realizar o Health Check de cada GE <br>
<a href="https://github.com/fabiocabrini/fiware/blob/main/FIWARE.postman_collection.json">Clique aqui<a/> para acessar a collection do Postman e baixar o arquivo cru
### 2. Importando a collection para o Postman
Após ter baixado o arquivo cru, abra o Postman, vá na aba de import e selecione o arquivo baixado.
### 3. Dando o Health check
- Vá na aba de cada um dos GEs (Iot Agent MQTT, Orion context Broker, STH-Comet)
- Selecione "Health Check"
- No lugar de URL coloque o IP do servidor (Nesse caso estamos criando um servidor local. Por conta disso, basta digitar "localhost")
![health_check_explicacao](https://github.com/gabrieldfr/sprint3_edge/assets/127216244/b5098576-30dd-4e24-9670-14b36944ef19)

- Agora clique em "send" e verifique a resposta do Postman
![health_check_explicacao_2](https://github.com/gabrieldfr/sprint3_edge/assets/127216244/16fdce7f-f63f-40c4-bb67-855d91a85ce4)
# Recebendo e enviando informações
- Para exemplificar essa parte, nós iremos utilizar o nosso projeto em desenvolvimento, relacionado ao monitoramento da luminosidade, umidade e temperatura ambiente para o armazenamento seguro de vinhos
## Requistos mínimos de hardware para o nosso projeto
- Esp32
- DHT 11
- Sensor de luz (fotoresistor)

## Código para o esp32
- Para acessar nosso código, vá para esse arquivo em nosso repositório: https://github.com/gabrieldfr/checkpoint5_edge/blob/main/CodigoEsp32.c
## Mudanças a serem feitas no código
- Linha 17: Mude "Your broker adress" pelo endereço de IP do seu broker
- Linha 25: Mude "YourWiFiSSID" pelo nome do wifi que você estará utilizando para conectar o esp32 com a rede
- Linha 26: Mude "YourWifiPassword" pela senha do seu Wifi
## Código em python para realizar a leitura dos dados do servidor obtidos pelo ESP32
- Para acessar nosso código, vá para esse arquivo em nosso repositório: https://github.com/gabrieldfr/checkpoint5_edge/blob/main/LeituraDosDadosServidor.py
- Para fornecer esses dados vitais, contamos com a biblioteca requests. Ela age como nossa "ponte de comunicação" com o servidor, permitindo-nos buscar, de forma eficiente, as informações relevantes para nossa análise. A biblioteca requests desempenha um papel fundamental ao coletar, armazenar e tornar acessíveis esses dados.
- Além disso, nosso projeto oferece a funcionalidade adicional de representação visual dos dados coletados. Isso é realizado com a ajuda da biblioteca matplotlib. Com essa ferramenta poderosa, podemos criar gráficos detalhados e informativos que auxiliam na interpretação das condições do ambiente.
