# Ristretto


*Ristretto* est un compilateur qui contient 3 types (*int*, *bool* et *string*) et deux fonctions (*print* et *println*). Ristretto reconnaît la définition et l’initialisation d’une variable en même temps, mais ne reconnaît que des variables globales. Les opérations basiques (+, -, *, /, ==) sont implémentées lors de l’initialisation d’une variable et à l’intérieur d’un appel à *print* ou *println* pour des valeurs allant de 0 à 5. print et println gèrent les int de 0 à 5 et variables globales.

Ristretto utilise *Java 8*, soit la version *52.00*.

## Utilisation

Après avoir compilé le programme correctement, il est utilisable de cette façon : 

```
./ristretto main.ris
```

*ristretto* produit en sortie *Main.class* contenant du bytecode exécutable par *Java*.

Le fichier *.ris* doit absolument contenir la fonction principale

```
// main function
void main(void) {

}
```

qui est le strict minimum, une erreur se produira sinon. Les variables sont toutes globales, et donc définies et initialisées au tout début du fichier. Les fonctions *print* et *println* ne doivent être appellées qu’à l’intérieur de la fonction main.

## Exemples

Voici un simple exemple :

```
// hello.ris
string s = "Hello";
int a = 122;
bool b = true; // true == 1, false == 0
void main(void) {
println s;
println a;
println b;
}
```

```
$ java Hello
Hello
122
1
```

4 opérations arithmétiques de base ainsi que les opérations AND et OR sont aussi possibles :

```
int a = 2 + 99;
int b = 128 / 4;
int c = 30 - 21;
bool d = true && false;
bool d2 = true || false;
void main(void) {
println a;
println b;
println c;
print d;
print d2;
}
```

```
$ java Test
101
32
9
01
```

Des calculs plus complexe sont également compréhensible:

```
//
36 == 36
int isTrue = ((5 * 3) + (4 * 6) - 3) == (6 * (5 + (true == (97 - 96))));
int b = 3 + (true && true);
void main(void) {
println b;
println isTrue;
}
```

```
$ java Test2
4
1
```
