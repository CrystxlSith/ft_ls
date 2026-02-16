# Pointeurs en C - l'essentiel

## Une variable c'est une boite

```
int x = 42;

Memoire:
[adresse 0x100]  ->  | 42 |   <- c'est x
```

- `x` = la valeur (42)
- `&x` = l'adresse (0x100)

---

## Un pointeur c'est une variable qui stocke une adresse

```
int x = 42;
int *p = &x;

[0x100]  ->  | 42 |      <- x
[0x200]  ->  | 0x100 |   <- p (contient l'adresse de x)
```

- `p` = l'adresse de x (0x100)
- `*p` = la valeur a cette adresse (42)
- `&x` = l'adresse de x (0x100)

---

## `*` a deux sens differents

```c
int *p;      // declaration : "p est un pointeur vers int"
*p = 42;     // utilisation : "la valeur a l'adresse p"
```

Ne pas confondre les deux. Meme symbole, sens different selon le contexte.

---

## `->` vs `.`

```c
t_options opts;       // struct sur la pile
opts.l = 1;           // acces direct avec .

t_options *ptr = &opts;
ptr->l = 1;           // acces via pointeur avec ->
(*ptr).l = 1;         // equivalent, mais personne ecrit ca
```

**Regle simple :**
- Variable normale -> `.`
- Pointeur -> `->`

---

## Pourquoi `&opts` dans `parseArgs(argc, argv, &opts)` ?

```c
// dans main :
t_options opts;                    // la struct vit dans main
parseArgs(argc, argv, &opts);     // on passe son ADRESSE

// dans parseArgs :
void parseArgs(..., t_options *opts)  // la fonction recoit l'adresse
{
    opts->l = 1;   // modifie la VRAIE struct de main
}
```

Sans `&`, la fonction recevrait une **copie**. Les modifications seraient perdues au retour.

```
AVEC &opts :
main:    [opts] <--------- parseArgs modifie directement ici
              ^
              |
parseArgs:  [ptr] pointe vers la vraie struct

SANS & (copie) :
main:    [opts]            <- jamais modifie !
parseArgs:  [copie de opts] <- modifie la copie, perdue au retour
```

---

## Tableau = pointeur vers le premier element

```c
char *tab[3] = {"hello", "world", NULL};

tab    -> pointe vers tab[0]
tab[0] -> pointe vers "hello"
tab[1] -> pointe vers "world"
tab[2] -> NULL
```

`char **tab` = un pointeur vers un pointeur vers des chars = un tableau de strings

---

## `char *` vs `char **`

```
char *str = "hello";

str -> | 'h' | 'e' | 'l' | 'l' | 'o' | '\0' |


char **tab = {"hello", "world"};

tab -> | ptr1 | ptr2 | NULL |
         |       |
         v       v
       "hello" "world"
```

- `char *` = une string
- `char **` = un tableau de strings (comme argv)

---

## `const` et les pointeurs

```c
char const *str;         // la STRING est constante, le pointeur non
                         // str = "autre";    OK
                         // str[0] = 'X';     INTERDIT

char *const str;         // le POINTEUR est constant, la string non
                         // str = "autre";    INTERDIT
                         // str[0] = 'X';     OK

char const *const str;   // les DEUX sont constants
                         // rien n'est modifiable
```

Pour `char const **paths` :
- `paths[0] = argv[1];`   -> OK (on change le pointeur)
- `paths[0][0] = 'X';`    -> INTERDIT (on modifie la string)

---

## Resume des operateurs

| Operateur | Ce que ca fait | Exemple |
|-----------|---------------|---------|
| `&x` | donne l'adresse de x | `int *p = &x;` |
| `*p` | donne la valeur a l'adresse p | `int val = *p;` |
| `p->champ` | accede au champ via un pointeur | `opts->l = 1;` |
| `x.champ` | accede au champ directement | `opts.l = 1;` |
| `p[i]` | equivalent a `*(p + i)` | `argv[1]` |

---

## La regle d'or

Un pointeur **doit** pointer vers quelque chose de valide :

| Source | Exemple |
|--------|---------|
| Une variable existante | `int *p = &x;` |
| De la memoire allouee | `int *p = malloc(sizeof(int));` |
| Le retour d'une fonction | `struct dirent *de = readdir(dir);` |
| NULL (mais ne jamais lire/ecrire) | `int *p = NULL;` |

Sinon -> **segfault**.

---

## Piege classique : pointeur non initialise

```c
int *p;        // p pointe vers n'importe ou (adresse poubelle)
*p = 42;       // SEGFAULT : ecrit dans le vide

// vs

int x;
int *p = &x;   // p pointe vers x
*p = 42;       // OK : x vaut maintenant 42
```

C'est exactement le probleme que tu as eu avec `t_options *des; des->R = 1;`
