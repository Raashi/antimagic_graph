from sys import argv
from io import BytesIO

from docx import Document
import matplotlib.pyplot as plt
import networkx as nx


def main():
    with open(argv[1]) as f:
        data = f.read()

    doc = Document()

    list_g6 = data.split('\n')[:-1]
    for g6 in list_g6:
        bio = BytesIO(g6.encode())
        graph = nx.read_graph6(bio)
        nx.draw(graph, with_labels=True, node_size=2000, node_color='w', pos=nx.circular_layout(graph))
        ax = plt.gca()
        ax.collections[0].set_edgecolor('black') 

        buffer = BytesIO()
        plt.savefig(buffer, format='png')
        plt.clf()
        plt.cla()
        plt.close()
        doc.add_picture(buffer)

    doc.save('g.docx')


if __name__ == '__main__':
    main()
