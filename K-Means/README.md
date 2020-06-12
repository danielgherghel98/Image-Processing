## Descriere proiect


   Scopul acestui proiect este realizarea segmentarii unei imagini si clasificarea acesteia in mai multe multe grupuri utilizand metoda K-Means clustering. Noi trebuie sa realizam clusterizarea imaginilor color utilizand mai multe metrici ( euclidiana, cosinus,  L1 - Manhattan).  

   Segmentarea imaginii este procesul de partiționare a unei imagini digitale în mai multe regiuni distincte care conțin fiecare pixel (seturi de pixeli, cunoscute și sub numele de superpixeli) cu atribute similare. Rolul segmentării imaginii este de a schimba reprezentarea acesteia în ceva mai semnificativ și mai ușor de analizat. Acesta este procesul de atribuire a unei etichete fiecărui pixel dintr-o imagine, astfel încât pixelii cu aceeași etichetă au anumite caracteristici.

   Algoritmul de clustering K-Means este un algoritm nesupravegheat și este utilizat pentru a segmenta zona de interes de pe fundal. Acesta grupează sau compartimentează datele date în K-grupuri sau părți bazate pe K-centroids.

Pași în algoritmul K-Means:
1. Alegem numărul de clustere K
2. Selectam la puncte K aleatorii, centroizii 
3. Alocam fiecare punct de date celui mai apropiat centroid → care formează clusterul K
4. Calculam și așezam noul centroid al fiecărui cluster
5. Reasignam fiecare punct de date către noul centroid cel mai apropiat. Daca a avut loc vreo reasignare, ne intoarcem la pasul 4, în caz contrar, modelul este gata.

   Am construit o structura Punct ce contine un vector pentru a reprezenta coordonatele x, y, R,G,B, H,S,V dar si un cluster intreg care este o modalitate de reprezenta ca un punct apartine unui cluster specific. 

   Functia extractFeatures are rolul de a extrage int-un vector, toate caracteristicile punctelor din imagine (x, y, R,G,B, H,S,V). 
   Functia kMeansClustering realizeaza algoritmul K-means si are ca parametrii vectorul de puncte determinat anterior, numarul de clustere, numarul de repetitii, weight-ul ales, eroarea si distanta pe care o vom aplica( Euclidiana, Cosinus sau Manhattan). Cu cat numarul de repetitii este mai mare, solutia final a va fi mai buna. Dacă distanța dintre un punct și un cluster curent este mai mică decât distanța dintre acest punct și clusterul anterior, actualizam punctul pentru a face parte din clusterul current. După prima iterație punctele nu vor fi distribuite în mod egal fiecărui grup, asadar  trebuie să existe o a doua parte în care se calculează noii centroizi, realizat prin apelarea metodei computeCentroids.
  ComputeCentroids itereaza peste puncte pentru a face legatura intre date(puncte) si centroizi si construieste noii centrozi.
  Pentru afisarea rezultatului folosim functia generateKMeansResult si ne vom folosi de imaginea originala, de vectorul de puncte cu caracteristicile si de un vector de puncta cu centroizii calculate cu kMeansClustering(). Aici vom construi imaginea destinatie si o vom afisa in terminal.
   Pentru calcularea distantelor vom folosi 3 distante: Euclidiana, Manhattan si Cosinus. Toate functiile vor avea nevoie de 2 puncte si de vectorul de ponderi constante. Distanta Euclidiana măsoară lungimea unui segment care leagă cele două puncta.
   Distanta Manhattan reprezinta distanța dintre două puncte dintr-o grilă bazată pe o cale strict orizontală și / sau verticală (adică de-a lungul liniilor de grilă). 
  Similaritate cosinus este folosita in general ca metrică pentru măsurarea distanței atunci când magnitudinea vectorilor nu contează. Aceasta are valori intre -1 si 1, -1 insemand ca nu exista similaritate intre cei 2 vectori, 1 insemnand ca sunt exact la fel.

Imaginea sursa pe care am facut testele: <br />
![kmeansSrc](https://user-images.githubusercontent.com/44845472/83761916-9d663500-a67f-11ea-9fd2-79dcf141d3d6.PNG)
<br />
Imaginile rezultate, corespunzatoare distantei euclidiene, cosine similarity si distantei Manhattan, pentru K=8 clustere: <br />
![kmeans8](https://user-images.githubusercontent.com/44845472/83762235-fe8e0880-a67f-11ea-98dd-ecf8b6877804.PNG)
<br />
Imaginile rezultate, corespunzatoare distantei euclidiene, cosine similarity si distantei Manhattan, pt. K=20 clustere: <br />
![kmeans20](https://user-images.githubusercontent.com/44845472/83762410-2bdab680-a680-11ea-8455-8fefce7ea928.PNG)
<br />

Imaginile rezultate, corespunzatoare distantei euclidiene, cosine similarity si distantei Manhattan, pt. K=50 clustere: <br />
![kmeans50](https://user-images.githubusercontent.com/44845472/83762486-44e36780-a680-11ea-98b0-3c8678a56137.PNG)
<br />

Imaginile rezultate, corespunzatoare distantei euclidiene, cosine similarity si distantei Manhattan, pt. K=100 clustere: <br />
![kmeans100](https://user-images.githubusercontent.com/44845472/83762547-52005680-a680-11ea-8208-8abb1fd71c40.PNG)
<br />

Imaginile rezultate, corespunzatoare distantei euclidiene, cosine similarity si distantei Manhattan, pt. K=200 clustere: <br />
![kmeans200](https://user-images.githubusercontent.com/44845472/83762604-62183600-a680-11ea-9465-68fd5eaf3c0c.PNG)
<br />

TUTORIAL GIT


Proiectul e facut sa "functioneze" cu git.

Pentru a lucra:

1) Cititi tutorialul de git si ssh: http://users.utcluj.ro/~visoft/tutoriale/
2) Faceti-va cont pe github daca nu aveti
3) Va setati ssh-ul sa puteti accesa github-ul
4) Va puneti pe calculator git-scm
5) Va clonati proiectul specificat de profesor:
  - Deschideti o consola ``Git CMD``
  - Navigati unde vreti sa salvati proiectul
  - Rulati ``git clone URL`` unde ``URL`` este adresa repository-ului accesat prin SSH.
6) In folderul proiectului va dezarhivati fisierele din aceasta arhiva: [primiti link separat pe grup]
7) Deschideti Visual Studio, navigati la folderul proiectului, si deschideti proiectul. Upgradati daca vreti.
8) Compilati.
9) Urmariti tutorialul video despre cum sa folositi git-ul din Visual Studio.


Modificari in Visual studio:

Langa Solution Explorer exista un Team explorer. Mergeti pe iconita de Home si alegeti Settings.
Mergeti pana la Diff & Merge tool si alegeti Visual Studio.

Daca nu merge sa interactionati cu repository-ul remote:

Intrati in fisierul ``.gitconfig`` din folderul home (C:\users\xxx) si adaugati in sectiunea ``[core]``:

	sshCommand = ssh.exe

Daca tot nu merge, deschideti o consola si incercati sa dati comanda ``git fetch --all`` in folderul proiectului. Daca va da erori de cheie, adaugati in variabilele de sistem (per user) ``GIT_SSH=C:\Windows\System32\OpenSSH\ssh.exe``


Spre deosebire de tutorialul de git, vom lucra direct in master, fara sa mai facem branch-uri. Deasemenea, cautati pe google
despre *rebase*. NU il recomand a fi folosit, decat cu atentie.

