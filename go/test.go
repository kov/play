package main

import "fmt"

type MeuPau struct {
     tamanho int
}

func seucu() {
     fmt.Printf("lalala\n")
}

func main() {
        s := "meu pau";
        var p *string = &s;
        *p = "se fode";
	fmt.Printf("Hello World! %s - %s\n", s, *p);

        mp := new(MeuPau);
        mp.tamanho = 10;
        fmt.Printf("Tamanho: %d\n", mp.tamanho);

        seucu();
}
