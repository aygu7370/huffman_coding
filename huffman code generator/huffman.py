###################################################################################
# @file huffman.py
################################################################################### 
# Date:        04-30-2022
# Author:      Ayush Gupta
# Description: This file convert the given list of strings in their characters and 
#              their respective encoded Huffman codes.
# Refernce:    https://www.programiz.com/dsa/huffman-coding
###################################################################################

from collections import Counter
endodedHuffmanFile = "encodeHuffman.txt"
dedodedHuffmanFile = "decodeHuffman.txt"

class NodeTree(object):
    def __init__(self, left=None, right=None):
        self.left = left
        self.right = right

    def children(self):
        return self.left, self.right

    def __str__(self):
        return self.left, self.right


def huffman_code_tree(node, binString=''):
    '''
    Function to find Huffman Code
    '''
    if type(node) is str:
        return {node: binString}
    (l, r) = node.children()
    d = dict()
    d.update(huffman_code_tree(l, binString + '0'))
    d.update(huffman_code_tree(r, binString + '1'))
    return d


def make_tree(nodes):
    '''
    Function to make tree
    :param nodes: Nodes
    :return: Root of the tree
    '''
    while len(nodes) > 1:
        (key1, c1) = nodes[-1]
        (key2, c2) = nodes[-2]
        nodes = nodes[:-2]
        node = NodeTree(key1, key2)
        nodes.append((node, c1 + c2))
        nodes = sorted(nodes, key=lambda x: x[1], reverse=True)
    return nodes[0][0]


if __name__ == '__main__':

    #read the cprpus and add a few \0 lines to complete the collection
    f = open("corpus.txt", "r")  
    corpus = f.readlines()
    corpus.append("\0\r\n")
    corpus.append("\0\r\n")
    corpus.append("\0\r\n")
    corpus.append("\0\r\n")
    corpus.append("\0\r\n")
    string = "".join(corpus)
    
    #close the file
    f.close()
    
    #initialize new lists for both encoded and decoded huffman tables
    encodeHuffmanTable = []
    decodeHuffmanTable = []

    #get the frequency of each symbol
    freq = dict(Counter(string))
    #sort the dictionary on the basis of frequency
    freq = sorted(freq.items(), key=lambda x: x[1], reverse=True)

    #build huffman tree on the basis of frequencies
    node = make_tree(freq)
    #traverse each node and store the path in encoding list
    encoding = huffman_code_tree(node)
    for i in encoding:
        #append data in the encoded and decoded lists
        encodeHuffmanTable.append("{'" + f'{i}' + "',0b" + f'{encoding[i]}' + "," + f'{len(encoding[i])}' + "},\n")
        decodeHuffmanTable.append("{'" + f'{i}' + "'," + '"'+ f'{encoding[i]}' + '"' +"," + f'{len(encoding[i])}' + "},\n")
  
    # store the encoded huffman in encodeHuffman.txt
    fe = open(endodedHuffmanFile, "w")
    fe.writelines(encodeHuffmanTable)
    #close the file
    fe.close()

    # store the decoded huffman in encodeHuffman.txt
    fd = open(dedodedHuffmanFile, "w")
    fd.writelines(decodeHuffmanTable)
    #close the file
    fd.close()