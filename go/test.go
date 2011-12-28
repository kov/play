package main

import "fmt"

type MeuPapagaio struct {
     tamanho int
}

func seuouvido() {
     fmt.Printf("lalala\n")
}

func main() {
        s := "meu papagaio";
        var p *string = &s;
        *p = "se ame";
	fmt.Printf("Hello World! %s - %s\n", s, *p);

        mp := new(MeuPapagaio);
        mp.tamanho = 10;
        fmt.Printf("Tamanho: %d\n", mp.tamanho);

        seuouvido();
}
