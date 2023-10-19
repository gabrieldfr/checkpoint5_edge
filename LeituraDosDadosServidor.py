import requests
import matplotlib.pyplot as plt

# Função para obter os dados a partir da API
def obter_dados(lastN, attribute):
    url = f"http://46.17.108.113:8666/STH/v1/contextEntities/type/Lamp/id/urn:ngsi-ld:Lamp:001/attributes/{attribute}?lastN={lastN}"

    headers = {
        'fiware-service': 'smart',
        'fiware-servicepath': '/'
    }

    response = requests.get(url, headers=headers)

    if response.status_code == 200:
        data = response.json()
        data_list = data['contextResponses'][0]['contextElement']['attributes'][0]['values']
        return data_list
    else:
        print(f"Erro ao obter dados: {response.status_code}")
        return []

# Função para criar e exibir o gráfico
def plotar_grafico(data_list, attribute, ylabel, color):
    if not data_list:
        print("Nenhum dado disponível para plotar.")
        return

    values = [entry['attrValue'] for entry in data_list]
    tempos = [entry['recvTime'] for entry in data_list]

    plt.figure(figsize=(12, 6))
    plt.plot(tempos, values, marker='o', linestyle='-', color=color)
    plt.title(f'Gráfico de {attribute} em Função do Tempo')
    plt.xlabel('Tempo')
    plt.ylabel(ylabel)
    plt.xticks(rotation=45)
    plt.grid(True)

    plt.tight_layout()
    plt.show()

# Solicitar ao usuário um valor "lastN" entre 1 e 100
while True:
    try:
        lastN = int(input("Digite um valor para lastN (entre 1 e 100): "))
        if 1 <= lastN <= 100:
            break
        else:
            print("O valor deve estar entre 1 e 100. Tente novamente.")
    except ValueError:
        print("Por favor, digite um número válido.")

# Obter os dados de luminosidade, temperatura e umidade e plotar os gráficos
luminosity_data = obter_dados(lastN, "luminosity")
temperature_data = obter_dados(lastN, "temperature")
humidity_data = obter_dados(lastN, "humidity")

plotar_grafico(luminosity_data, "Luminosidade", "Luminosidade", 'r')
plotar_grafico(temperature_data, "Temperatura", "Temperatura (°C)", 'b')
plotar_grafico(humidity_data, "Umidade", "Umidade (%)", 'g')