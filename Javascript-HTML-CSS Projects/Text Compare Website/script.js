// COMP10003 Mini Project - Text Compare
// Jesslyn Andriono - 1368995
// Semester 1 2023 - 26 May 2023

// Declare constants
const RESET = "reset";
const REFRESH = "refresh";
const SENTENCE_ENDS = [".", "!", "?"];

/* Takes nothing, processes the texts in text area 1 and 2, shows the cosine 
similarity, word count table, and text stats on the HTML page, returns nothing */
function compare() {
    // Reset cosine similarity, word count table, and text stat values
    reset(REFRESH);

    // Create a variable containing the id of text 1 and text 2
    let textId1 = "#text1";
    let textId2 = "#text2";

    /* Remove non-Alphanumeric characters and compiles the 
    words in the text into an array */
    let wordArray1 = prepText(textId1);
    let wordArray2 = prepText(textId2);

    // Create a sorted array of every unique word from both texts
    let combinedWords = uniqueWords(wordArray1, wordArray2);
    
    // Create a word counter and reset each text's words counts to 0
    let wordCountArray = [];
    for (i = 0; i < combinedWords.length; i++) {
        wordCountArray.push({word: combinedWords[i], text1: 0, text2: 0});
    }

    // Increment word counts of each text every time it is found
    for (i = 0; i < wordArray1.length; i++) {
        if (found = wordCountArray.find(element => element.word == wordArray1[i])) {
            found.text1++; 
        }
    }

    for (i = 0; i < wordArray2.length; i++) {
        if (found = wordCountArray.find(element => element.word == wordArray2[i])) {
            found.text2++; 
        }
    }

    // Calculate the cosine similarity and round to two decimal places
    let textSimilarity = cosineSimilarity(wordCountArray) * 100;
    textSimilarity = parseFloat(textSimilarity.toFixed(2));
    
    // If text similarity value is valid, show the texts' stats
    if (!Number.isNaN(textSimilarity)) {
        showCosineSim(textSimilarity);
        showTableCount(wordCountArray);
        showWordCount("#countSpan1", wordArray1);
        showWordCount("#countSpan2", wordArray2);
        showCharCount("#charSpan1", textId1);
        showCharCount("#charSpan2", textId2);
        showReadability("#readability1", textId1, wordArray1);
        showReadability("#readability2", textId2, wordArray2);
    } else {
       // If text similarity value invalid, show error message and style
        showError(wordArray1, wordArray2);
    }

    return; 
}

// Takes the id of a text and returns an array of words from the text
function prepText(textId) {
    // Remove non-Alphanumeric characters and multiple spaces from the text
    let text = document.querySelector(textId).value;
    let string = text.replace(/[^\w\s\']|_/g, "").replace(/\s{2,}/g, " ");
    string = string.trim().toLowerCase();

    // Convert string into an array of words
    let wordArray = string.split(" ");
    
    // Remove non-Alphanumeric characters that are still present such as quotes
    for (i = 0; i < wordArray.length; i++) {
        wordArray[i] = wordArray[i].replace(/['"]+/g, "");

        if (!isAlphanum(wordArray[i])) {
            wordArray.splice(i, 1);
            i = i - 1;
        }
    }

    return wordArray;
}

/* Takes a string, returns true if the string is entirely Alphanumeric, 
false otherwise */
function isAlphanum(string) {
    return string.match(/^[a-zA-Z0-9]+$/) !== null;
}

// Takes two arrays of words and returns an array of unique words from both text
function uniqueWords(wordArray1, wordArray2) {
    // Copy all words in the word array for text 2 into the array 'combinedWords'
    let combinedWords = [...wordArray2];

    // Add words from text 1 not present in text 2 to 'combinedWords'
    for (i = 0; i < wordArray1.length; i++) {
        if (!wordArray2.includes(wordArray1[i])) {
            combinedWords.push(wordArray1[i]);
        }
    }

    // Store unique words in an array
    let uniqueWords = []
    combinedWords = combinedWords.sort()
    for (i = 0; i < (combinedWords.length - 1); i++) {
        if (combinedWords[i] !== combinedWords[i + 1]) {
            uniqueWords.push(combinedWords[i]);
        }
    }
    
    uniqueWords.push(combinedWords[combinedWords.length - 1])

    return uniqueWords;
}

// Takes an array of word counts and returns the cosine similarity based on it
function cosineSimilarity(wordCountArray) {
    // Calculate the dot product
    let dotProduct = 0;
    for (i = 0; i < wordCountArray.length; i++) {
        dotProduct += (wordCountArray[i].text1 * wordCountArray[i].text2);
    }
    
    // Calculate the vector of magnitude
    let magnitude1 = vectorMagnitude(1, wordCountArray);
    let magnitude2 = vectorMagnitude(2, wordCountArray);

    // Calculate cosine similarity
    let similarity = dotProduct / (magnitude1 * magnitude2);
    
    return similarity;
}

// Takes a text number and word count array, returns the text's vector of magnitude
function vectorMagnitude(textNum, wordCountArray) {
    // Calculate sum of powers
    let powerSum = 0;
    for (i = 0; i < wordCountArray.length; i++) {
        if (textNum === 1) {
            powerSum += (wordCountArray[i].text1 ** 2);
        } else {
            powerSum += (wordCountArray[i].text2 ** 2);
        }
    }
    
    // Calculate vector of magnitude
    let magnitude = Math.sqrt(powerSum);

    return magnitude;
}

// Takes the id of a text and returns the number of sentences in it
function countSentence(textId) {
    let text = document.querySelector(textId).value;
    
    // Adds an end punctuation in case the text does not have any
    text = text + SENTENCE_ENDS[0];
    let count = 0;

    /* Increases sentence count when an end punctuation is found, 
    counts consecutive end punctuations as one count */
    for (i = 0; i < text.length; i++) {
        if (SENTENCE_ENDS.includes(text[i])) {
            count++;
            if ((i > 0) && (SENTENCE_ENDS.includes(text[i - 1]))){
                count--;
            }
        }
    }
    
    return count;
}

/* Takes a text's id and the array of words in the text, returns the text's 
U.S. grade-level readability based on the Coleman-Liau index (reference: 
https://readabilityformulas.com/coleman-liau-readability-formula.php, 
inspired by: https://cs50.harvard.edu/x/2023/psets/2/readability/) */
function readability(textId, wordArray) {
    // Count the number of alphanumeric characters
    let letterCount = 0;
    for (i = 0; i < wordArray.length; i++) {
        letterCount += wordArray[i].length;
    }

    // Count the average word length and number of sentences per 100 words
    let aveLetter = letterCount / wordArray.length * 100;
    let aveSentence = countSentence(textId) / wordArray.length * 100;

    // Formula to calculate readability based on the Coleman-Liau index
    let index = (0.0588 * aveLetter) - (0.296 * aveSentence) - 15.8;

    return Math.round(index);
}

/* Takes a text's readability div destination id, text id, and the array of words 
in the text, shows the readability of the text on page, returns nothing */
function showReadability(destId, textId, wordArray) {
    let textReadability = document.createElement('span');
    
    /* If index is less than 1 or more than 16 (final year of 
    undergraduate studies) show a specified grade level, otherwise show
    the text's grade level on the HTML page*/
    if (readability(textId, wordArray) < 1) {
        textReadability.textContent = "Reading Level: Before Grade 1";
    } else if (readability(textId, wordArray) > 15) {
        textReadability.textContent = "Reading Level: Grade 16+";
    } else {
        textReadability.textContent = "Reading Level: Grade " + readability(textId, wordArray);
    }
    
    document.querySelector(destId).appendChild(textReadability);

    return;
}

// Takes a texts' cosine similarity and shows it on the HTML page, returns nothing
function showCosineSim(similarity) {
    let cosineSim = document.createElement('div');
    cosineSim.textContent = `Cosine Similarity: ${similarity}%`;
    document.querySelector("#cosineSim").appendChild(cosineSim);

    return;
}

/* Takes the combined word count array of the two texts, 
shows it on the page as a table, returns nothing */
function showTableCount(wordCountArray) {
    // Add table header element
    let firstRow = document.createElement('tr');
    let word = document.createElement('th');
    word.textContent = "Word";
    let text1 = document.createElement('th');
    text1.textContent = "Text 1";
    let text2 = document.createElement('th');
    text2.textContent = "Text 2";
    firstRow.appendChild(word);
    firstRow.appendChild(text1);
    firstRow.appendChild(text2);
    document.querySelector("#wordCount").appendChild(firstRow);

    // For every word in word count, add a row of the word and its word counts
    for (i = 0; i < wordCountArray.length; i++) {
        let newRow = document.createElement('tr');
        let td1 = document.createElement('td');
        td1.textContent = wordCountArray[i].word;
        let td2 = document.createElement('td');
        td2.textContent = wordCountArray[i].text1;
        let td3 = document.createElement('td');
        td3.textContent = wordCountArray[i].text2;
        newRow.appendChild(td1);
        newRow.appendChild(td2);
        newRow.appendChild(td3);
        document.querySelector("#wordCount").appendChild(newRow);
    }

    return;
}

/* Takes the two texts' respective word arrays, shows an error message 
if either of the texts are empty, returns nothing */
function showError(wordArray1, wordArray2) {
    let error = document.createElement('div');
    error.textContent = "Please input valid text into both boxes.";
    document.querySelector("#error").appendChild(error);

    let input = [wordArray1, wordArray2];
    let empty = [];

    // Determine which texts, if any, are empty
    for (i = 0; i < input.length; i++) {
        if (input[i].length == 0) {
            empty.push(i + 1);
        }
    }

    // If a text area is empty, assign the class of 'emptyTextArea'
    for (i = 0; i < empty.length; i++) {
        let areaId = "#area" + empty[i];
        document.querySelector(areaId).className = "emptyTextArea";
    }
    
    return;
}

/* Takes a texts's word count div destination id and its array of words, 
shows the text's word count on the HTML page, returns nothing */
function showWordCount(destId, wordArray) {
    let wordCount = document.createElement('span');
    wordCount.textContent = "Word Count: " + wordArray.length;
    document.querySelector(destId).appendChild(wordCount);

    return;
}

/* Takes a texts's character count div destination id and the text's id, 
shows the text's character count on the HTML page, returns nothing */
function showCharCount(destId, textId) {
    let text = document.querySelector(textId).value;
    let charCount = document.createElement('span');
    charCount.textContent = "Character Count: " + text.length;
    document.querySelector(destId).appendChild(charCount);

    return;
}

// Takes a reset condition (reset or refresh) and erases the HTML page's contents
function reset(condition) {
    // Reset cosine similarity
    let parent = document.querySelector("#cosineSim");
    parent.textContent = "";

    // Reset table content
    let table = document.querySelector("#wordCount");
    while (table.hasChildNodes()) {
    table.removeChild(table.lastChild);
    }

    // Reset text area color and error
    for (i = 0; i < 2; i++) {
        let areaId = "#area" + (i + 1);
        document.querySelector(areaId).className = "normalTextArea";
    }

    let error = document.querySelector("#error");
    error.textContent = "";

    // Reset word count
    for (i = 0; i < 2; i++) {
        let countId = "#countSpan" + (i + 1);
        count = document.querySelector(countId);
        count.textContent = "";
    }

    // Reset character count
    for (i = 0; i < 2; i++) {
        let charId = "#charSpan" + (i + 1);
        charCount = document.querySelector(charId);
        charCount.textContent = "";
    }

    // Reset readability level
    for (i = 0; i < 2; i++) {
        let readabilityId = "#readability" + (i + 1);
        textReadability = document.querySelector(readabilityId);
        textReadability.textContent = "";
    }

    // If condition is reset, empties the text area as well
    if (condition != REFRESH) {
        let text1 = document.querySelector("#text1");
        text1.value = "";
        let text2 = document.querySelector("#text2");
        text2.value = "";
    }

}