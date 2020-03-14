// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordsList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ProcessGuess(PlayerInput);
}

void UBullCowCartridge::SetupGame(){

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull and Cow Game"));
    PrintLine(TEXT("Guess the Hidden Word of %i letters."), HiddenWord.Len());
    PrintLine(TEXT("You have %i Lives."), Lives);
    PrintLine(TEXT("Hidden word is %s."), *HiddenWord);
}

void UBullCowCartridge::ProcessGuess(const FString& Guess){

    if(Guess == HiddenWord){
        PrintLine(TEXT("You win!!!"));
        EndGame();
        return;
    }

    if(Guess.Len() != HiddenWord.Len()){
            PrintLine(TEXT("Hidden word is %i letters long."), HiddenWord.Len());
    }

    if(!IsIsogram(Guess)){
        PrintLine(TEXT("No repeating letters. Guess again"));
        return;
    }
    --Lives;
    PrintLine(TEXT("You lost a life"));
    if(Lives < 1){
        PrintLine(TEXT("You lost all the lives."));
        EndGame();
        return;
    }
    FBullCowCount score = GetBullCow(Guess);
    PrintLine(TEXT("You have %i bulls and %i cows"), score.bulls, score.cows);
    PrintLine(TEXT("You have %i lives remaining\nTry again"), Lives);
}

void UBullCowCartridge::EndGame(){
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again"));
    ClearScreen();
    SetupGame();   
}

bool UBullCowCartridge::IsIsogram(const FString& Word)const{

    //i=index, j= comparison
    int32 i=0;
    for(i=0; i < Word.Len(); i++){
        for(int32 j=i+1; j< Word.Len(); j++){
            if(Word[i] == Word[j])
                return false;
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const{

    TArray<FString> ValidWords;

    for(FString Word : WordList){
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)){
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCow(const FString& Guess) const{

    FBullCowCount count;
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++) 
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex]){
            count.bulls++;
            continue;
        }
        for(int32 HiddenIndex = 0; HiddenIndex<HiddenWord.Len(); HiddenIndex++){
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex]){
                count.cows++;
                break;
            }
        }
    }
    return count;
}