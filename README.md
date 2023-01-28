# ALGORITMI PARALELI SI DISTRIBUITI

---

### Procesare paralela folosind paradigma Map-Reduce

### In Dragos-Cosmin - 336CC

## mapper

    M-am ajutat de o coada pentru a citi in mod dinamic fisierele, astfel reusind sa ii dau unui thread care a terminat mai repede un alt fisier de analizat. Pentru acest lucru am avut nevoie si de un mutex. Pe urma am continuat sa citesc fiecare linie din fisier si sa verific cu ajutorul functiei *binary_search* daca numarul meu respecta conditia. Daca da, il adaug in map-ul meu la key-ul corespunzator. De aceea am inceput si for-ul de la 2 pana la R + 1. Pe urma pun o bariera pentru a astepta toate thread-urile sa termine, iar abia dupa sa treaca la reducer.

## reducer

    Foarte important este sa pun o bariera pentru a astepta toate thread-urile de la mapper sa termine. Cel mai usor mod prin care sa scap de duplicate mi s-a parut sa folosesc un set. Asa ca am adaugat elementele in el si am afisat la final doar size-ul acestuia. A trebuit sa il si golesc la sfarsit pentru ca altfel se adauga la fiecare iteratie.

## get_args

    Functie care imi ia argumentele de la linia de comanda si le pune in variabilele din structura mea.

## read_input_file

    Cu aceasta functie am citit doar fisierul test.txt din fiecare test si mi-am populat coada de string-uri.

## write_output_file

    Functia asta imi scrie in fisierele de output rezultatul. Am respectat formatul impus de enunt.

## binary_search

    Aceasta este functia care imi gaseste daca un numar este putere perfecta sau nu. Am folosit algoritmul de cautare binara pentru a avea o complexitate cat mai buna. Am mers doar pana la sqrt(n) pentru ca nu are rost sa caut mai departe. La fiecare iteratie verific daca numarul meu este mai mic sau mai mare decat pow(mijlocul, exp). La sfarsit daca numarul gasit la puterea exp este fix numarul initial, atunci il returnez, altfel returnez 0.

## main

    Aici am initializat bariera si mutex-ul, apoi am creat thread-urile de mapper si reducer in acelasi timp.
