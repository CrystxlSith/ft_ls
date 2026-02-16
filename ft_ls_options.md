# ft_ls - Guide des options obligatoires

## Vue d'ensemble

Tu dois recoder la commande `ls` avec 5 options obligatoires : `-l`, `-R`, `-a`, `-r`, `-t`.
Le comportement doit etre **identique** au vrai `ls`. Pas de gestion multi-colonnes sans `-l`, pas d'ACL/attributs etendus requis.

---

## Les 5 options mandatory

### `-l` (long format)

**Ce que ca fait :** Affiche les fichiers en format detaille, une entree par ligne.

**Exemple de sortie :**
```
total 24
drwxr-xr-x  2 jopfeiff staff  4096 Feb 10 14:30 src
-rw-r--r--  1 jopfeiff staff   234 Feb 10 14:30 Makefile
lrwxrwxrwx  1 jopfeiff staff    11 Feb 10 14:30 link -> target
```

**Les champs affiches (dans l'ordre) :**

| Champ | Description | Comment l'obtenir |
|-------|-------------|-------------------|
| Type + permissions | `drwxr-xr-x` | `stat()` / `lstat()` -> `st_mode` |
| Nombre de liens | `2` | `stat()` -> `st_nlink` |
| Proprietaire | `jopfeiff` | `getpwuid(st_uid)` |
| Groupe | `staff` | `getgrgid(st_gid)` |
| Taille | `4096` | `stat()` -> `st_size` |
| Date de modification | `Feb 10 14:30` | `stat()` -> `st_mtime` + `ctime()` |
| Nom du fichier | `src` | `readdir()` -> `d_name` |

**Le champ type (premier caractere) :**
- `-` : fichier regulier
- `d` : repertoire
- `l` : lien symbolique (afficher `nom -> cible` avec `readlink()`)
- `c` : character device
- `b` : block device
- `p` : pipe (FIFO)
- `s` : socket

**Le "total" :** C'est la somme des blocs (en unites de 512 octets, champ `st_blocks`) de tous les fichiers du repertoire. Affiche en premiere ligne.

**Implications pour le code :**
- Tu dois appeler `lstat()` sur **chaque** fichier (pas `stat()`, sinon tu suis les liens symboliques)
- Tu dois calculer le **padding** (alignement des colonnes) : parcourir tous les fichiers une premiere fois pour trouver les largeurs max de chaque colonne, puis afficher
- Pour les liens symboliques : `readlink()` pour lire la cible
- Pour la date : `ctime()` renvoie une string de 26 chars, tu dois extraire le morceau utile. Si le fichier a plus de 6 mois ou est dans le futur : afficher l'annee au lieu de l'heure

---

### `-R` (recursif)

**Ce que ca fait :** Liste recursivement tous les sous-repertoires.

**Exemple de sortie :**
```
.:
Makefile  src  libft

./src:
main.c

./libft:
libft.h  ft_strlen.c
```

**Implications pour le code (IMPORTANT - le sujet insiste la-dessus) :**
- Pense a `-R` **des le debut** de ton architecture. Si tu ne le fais pas, tu devras tout refactoriser
- Pour chaque repertoire rencontre, tu dois re-lister son contenu (approche recursive ou avec une pile/file)
- Affiche le chemin du repertoire suivi de `:` avant son contenu
- Separe chaque bloc de repertoire par une ligne vide
- **Ne PAS suivre** les liens symboliques (utilise `lstat`, pas `stat`)
- **Ignorer** `.` et `..` pour eviter les boucles infinies
- L'ordre de parcours doit respecter le tri en cours (alphabetique par defaut, ou par date si `-t`, inverse si `-r`)

**Conseil d'architecture :** Utilise une fonction qui prend un chemin et qui :
1. Ouvre le repertoire (`opendir`)
2. Lit les entrees (`readdir`)
3. Les stocke et les trie
4. Les affiche
5. Pour chaque sous-repertoire trouve, rappelle cette meme fonction

---

### `-a` (all)

**Ce que ca fait :** Affiche **tous** les fichiers, y compris les fichiers caches (ceux qui commencent par `.`).

**Sans `-a` :**
```
Makefile  src  libft
```

**Avec `-a` :**
```
.  ..  .git  .gitignore  Makefile  src  libft
```

**Implications pour le code :**
- Par defaut, `readdir()` te donne **tous** les fichiers, y compris `.` et `..` et les fichiers caches
- Sans `-a`, tu dois **filtrer** les entrees dont le nom commence par `.`
- Avec `-a`, tu affiches tout sans filtre
- `.` et `..` sont inclus avec `-a`
- Avec `-l -a`, le "total" doit aussi inclure les blocs de `.` et `..`

---

### `-r` (reverse)

**Ce que ca fait :** Inverse l'ordre de tri.

**Sans `-r` :** tri alphabetique A -> Z (ou du plus recent au plus ancien avec `-t`)
**Avec `-r` :** tri alphabetique Z -> A (ou du plus ancien au plus recent avec `-t`)

**Implications pour le code :**
- Ca s'applique a **tous** les tris : alphabetique par defaut, et par date avec `-t`
- Le plus propre : code ton tri normalement, puis a la fin inverse le resultat (ou change le sens du comparateur)
- Astuce : un simple flag qui multiplie le retour de ta fonction de comparaison par `-1`

---

### `-t` (sort by time)

**Ce que ca fait :** Trie les fichiers par date de modification (le plus recent en premier) au lieu du tri alphabetique par defaut.

**Implications pour le code :**
- Utilise `st_mtime` (et `st_mtim.tv_nsec` pour les nanoseconds si deux fichiers ont le meme timestamp en secondes)
- En cas d'egalite de date : trie par ordre alphabetique du nom
- Combinable avec `-r` : le plus ancien en premier
- Avec `-R`, chaque sous-repertoire est trie independamment par date

---

## Combinaisons d'options

Les options se combinent entre elles. Toutes les combinaisons sont valides :

| Combinaison | Effet |
|-------------|-------|
| `-la` | Long format + fichiers caches |
| `-lR` | Long format + recursif |
| `-lt` | Long format + tri par date |
| `-lr` | Long format + tri inverse |
| `-lRart` | Tout en meme temps |
| `-Rat` | Recursif + all + tri par date (sans long format = 1 nom par ligne) |

Les options peuvent etre :
- Groupees : `ft_ls -lRa`
- Separees : `ft_ls -l -R -a`
- Melangees : `ft_ls -lR -a -t`

---

## Fonctions autorisees et leur role

| Fonction | Role dans ft_ls |
|----------|-----------------|
| `write` | Toute sortie (stdout et stderr) |
| `opendir` | Ouvrir un repertoire pour le lire |
| `readdir` | Lire les entrees d'un repertoire une par une |
| `closedir` | Fermer un repertoire ouvert |
| `stat` | Obtenir les infos d'un fichier (suit les liens symboliques) |
| `lstat` | Obtenir les infos d'un fichier (ne suit PAS les liens symboliques) |
| `getpwuid` | Convertir un UID en nom d'utilisateur (pour `-l`) |
| `getgrgid` | Convertir un GID en nom de groupe (pour `-l`) |
| `listxattr` | Lister les attributs etendus (pas requis en mandatory) |
| `getxattr` | Lire un attribut etendu (pas requis en mandatory) |
| `time` | Obtenir le timestamp actuel (pour le format de date dans `-l`) |
| `ctime` | Convertir un `time_t` en string lisible |
| `readlink` | Lire la cible d'un lien symbolique (pour `-l`) |
| `malloc` | Allocation memoire |
| `free` | Liberation memoire |
| `perror` | Afficher un message d'erreur sur stderr |
| `strerror` | Obtenir la string d'erreur associee a errno |
| `exit` | Quitter le programme |

---

## `stat` vs `lstat` - Quand utiliser quoi ?

C'est un piege classique :

- **`lstat()`** : ne suit pas les liens symboliques. Te donne les infos **du lien lui-meme**
- **`stat()`** : suit les liens symboliques. Te donne les infos **du fichier cible**

**Regle generale pour ft_ls :** utilise **`lstat()`** partout. C'est le comportement par defaut de `ls`. Tu veux afficher les infos du lien, pas de sa cible.

---

## Structure `stat` - Les champs utiles

```c
struct stat {
    mode_t    st_mode;    // type + permissions (drwxr-xr-x)
    nlink_t   st_nlink;   // nombre de hard links
    uid_t     st_uid;     // user ID du proprietaire
    gid_t     st_gid;     // group ID
    off_t     st_size;    // taille en octets
    blkcnt_t  st_blocks;  // nombre de blocs 512 octets (pour "total")
    time_t    st_mtime;   // date de derniere modification
};
```

Pour decoder `st_mode` (permissions), utilise les macros :
```c
S_ISREG(m)   // fichier regulier ?
S_ISDIR(m)   // repertoire ?
S_ISLNK(m)   // lien symbolique ?
S_ISCHR(m)   // character device ?
S_ISBLK(m)   // block device ?
S_ISFIFO(m)  // pipe ?
S_ISSOCK(m)  // socket ?

// Bits de permissions
m & S_IRUSR  // read user
m & S_IWUSR  // write user
m & S_IXUSR  // execute user
m & S_IRGRP  // read group
// ... etc

// Bits speciaux
m & S_ISUID  // setuid -> 's' au lieu de 'x' pour user
m & S_ISGID  // setgid -> 's' au lieu de 'x' pour group
m & S_ISVTX  // sticky bit -> 't' au lieu de 'x' pour other
```

---

## Format de la date dans `-l`

`ctime()` renvoie : `"Wed Feb 10 14:30:00 2026\n"`

Tu dois afficher :
- **Si le fichier a moins de 6 mois** : `Feb 10 14:30` (mois jour heure:minutes)
- **Si le fichier a plus de 6 mois (ou est dans le futur)** : `Feb 10  2026` (mois jour annee, avec un espace supplementaire)

Utilise `time()` pour obtenir le timestamp actuel et comparer.

---

## Gestion des erreurs

- Si un fichier/repertoire n'existe pas : `ft_ls: <nom>: No such file or dir`
- Si tu n'as pas les permissions : `ft_ls: <nom>: Permission denied`
- Utilise `perror()` ou `strerror(errno)` pour generer le message
- Le programme **continue** malgre les erreurs (il ne quitte pas au premier probleme)
- Code de retour : `0` si tout va bien, `1` s'il y a eu au moins une erreur

---

## Ordre de traitement des arguments

Quand `ft_ls` recoit plusieurs arguments :
1. D'abord les **fichiers** (tries entre eux)
2. Puis les **repertoires** (tries entre eux, separes par des lignes vides)
3. Les erreurs (fichiers inexistants) sont affiches **en premier**, tries alphabetiquement

```
$ ft_ls inexistant fichier.c rep1 rep2
ft_ls: inexistant: No such file or dir
fichier.c

rep1:
contenu_rep1

rep2:
contenu_rep2
```

---

## Resume rapide

| Option | En une phrase |
|--------|--------------|
| `-l` | Affichage detaille (permissions, owner, taille, date) |
| `-R` | Descend recursivement dans les sous-dossiers |
| `-a` | Montre les fichiers caches (`.xxx`) |
| `-r` | Inverse le tri |
| `-t` | Trie par date de modification au lieu de l'alphabet |
