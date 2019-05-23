# Indie Studio


## Theory-craft Gameplay

![Bomberman](https://cdn03.nintendo-europe.com/media/images/06_screenshots/games_5/nintendo_switch_6/nswitch_superbombermanr/NSwitch_SuperBombermanR_01.jpg)

* Vue en plongée
* Génération procédurale
  * Le faire dans quel ordre ?
* Des powerups cachés dans les blocs
  * qui apparaissent random ?
  * qui apparaissent à la mort d'un joueur ?
* Un stade en 3D ?


### Liste des entités


* Joueurs (+ IA ?)
* Blocs de décoration (qui est indestructible ; contour de la map/intérieur/sol)
* Blocs destructibles (pouvant contenir un objet)
* Bombes (plusieurs types)
  * Classiques
  * Qui traversent les murs
  * Déclenchables à la main (?)
  * Sphériques
* Objets
  * Power-ups
    * Vitesse (de marche)
    * Stock (nombre de bombes)
    * Déflagration (rayon d'explosion)
    * Wall-Pass
  * Spécialisation (change le type de bombe)
  * Pouvoir spéciaux
    * Kick (donner un coup dans la bombe)
    * Kamikaze (trainée de bombe constante)


## On fé koa


- Trouver des assets Bomberman
- Event manager
  - Irrlicht
  - Customs
- Un personnage qui se déplace sur un plan 2D
  - Plusieurs joueurs
- Objets destructibles
- Bombes


## On l'applique à l'ECS


- Event
  - Composants
    - Callback (id) (quelle fonction appeler pour tel event)
- Déplacement (via Event)
  - Composants
    - Position (x, y)
    - Velocity (vx, vy)
  - Tags
    - Movable
  - Notes
    - Pas besoin de quad-tree, le joueur n'est pas libre de ses mouvements, il appartient à une tile.
- Bombes
  - Composants
    - &Position
    - &Velocity
    - Bomb (type [enum], time, range)
  - Tags
    - Destructible
  - Notes
    - Gère l'explosion des différents types de bombes
    - Le son de l'explosion possède une position et peut influer sur les manettes (vibrations)
- Joueur
  - Composants
    - &Position
    - &Velocity
    - Player (type [enum], alive)
- Rendu graphique
  - Composants
    - &Position
    - Model (texture)
- Sons/Musique
  - Composants
    - &Position
    - Sound (id)