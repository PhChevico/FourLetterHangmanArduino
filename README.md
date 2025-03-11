# FourLetterHangman for Arduino

This is a simple *FourLetterHangman* game developed for Arduino, where players guess 4-letter words from different categories. The game is based on a word puzzle, with masked letters represented by underscores (_), and the player has to guess the correct word by pressing buttons.

This was my first experience with C and Arduino, and it was a fun project to build in my first year of applied computer science!

## Game Overview

The game presents a 4-letter word that has its letters hidden behind underscores. The player is given a set of categories, such as animals, humans, countries, and things. The goal is to guess the word in as few attempts as possible by using the buttons to reveal the letters of the word one by one.

The game uses a 4x4 grid for each category, and the player can select a category and start guessing letters. For each correct letter guessed, the player progresses through the word.

## How to Play

1. **Start the Game**: The game starts by selecting a category. Use **Button 1** to cycle through the available categories:
    - **ANML**: Animals
    - **HMAN**: Human Body Parts
    - **CTRY**: Countries
    - **THNG**: Things
2. **Select a Category**: Press **Button 2** to confirm the selected category.
3. **Guess the Word**: The word is shown with some letters hidden as underscores (_). Use the following buttons to guess the letters:
    - **Button 0**: Increment the first character of the word if it's an underscore (_).
    - **Button 1**: Increment the third character of the word if it's an underscore (_).
    - **Button 2**: Increment the fourth character of the word if it's an underscore (_).
4. **Correct Guess**: When the word is fully guessed, the game will show the word and notify you with a buzzer sound. The number of attempts and the time taken are displayed.
5. **Restart the Game**: After finishing a puzzle, press **Button 1** to restart the game and select a new category.

---

This game was built using **C** and **Arduino**, using external libraries like `usart` for communication, `led` for visual output, `display` for the game screen, and `buttons` for interaction. It also utilizes the **AVR Timer** to track the time taken for each puzzle.
