//=================================================================================================
// Name        : encryptionthread.cpp
// Author      : Muhammad Obaidullah
// Version     : 0.1
// Copyright   : Electrical & Computer Engineering, Ryerson University ©2016
// Description : A class that encapsulates encryption algorithm
//=================================================================================================

#include "encryptionthread.h"

EncryptionThread::EncryptionThread(string inputFileName, string outputFileName, string key)
{
    this->inputFile = inputFileName;
    this->outputFile = outputFileName;
    vector<unsigned char> keyVector(key.begin(), key.end());
    this->key = keyVector;
}

void EncryptionThread::run()
{
    qDebug() << "Encryption worker thread: " << currentThreadId() << " spawned.";
    readAllBytes();
    output = CPU_ECB_PC_Encrypt(completeInputData, key);
}

void EncryptionThread::readAllBytes()
{
    completeInputData.clear();
    ifstream ifs(inputFile, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    vector<char>  completeInputData(pos);
    ifs.seekg(0, ios::beg);
    ifs.read(&completeInputData[0], pos);
}

vector<unsigned char> EncryptionThread::CPU_ECB_PC_Encrypt(vector<unsigned char> input, vector<unsigned char> key)
{
    // Step 1: Determining # of Rounds, # of Columns, and # of Words
    if(key.size() == 16) // 128-bit key
    {
        // 16 byte key has 10 rounds, and 4 32-bit words and 4 columns
        this->num_rounds    = 10;
        this->num_columns   = 4;
        this->num_words     = 4;
    }
    else
    {
        qDebug() << TAG << " : " << "Key not of correct/supported size.";
    }
    // Step 2: Expand the key for # of Rounds
    vector<unsigned char> expandedKey = expandKey(key);
    // Step 3: Pad the input if required
    padInput(&input);
    int blockSize = num_columns*num_words;
    uint totalBlocks = input.size() / blockSize;
    // Step 4: Go through each block and cipher it
    vector<unsigned char> output;
    for(int current_block = 0; current_block < totalBlocks; current_block++)
    {
        // This is to stop encryption if called from GUI
        QMutexLocker locker(&m_mutex);
        if (m_stop) break;
        // Copy the block from input to new block vector
        vector<unsigned char> block(input.at(current_block * blockSize), input.at(((current_block + 1) * blockSize) - 1));
        // Cipher the block
        vector<unsigned char> encryptedBlock = cipherBlock(block,expandedKey);
        // Add it to the output vector
        output.insert(output.end(), encryptedBlock.begin(), encryptedBlock.end());
    }
    // Step 5: Return the output vector
    return output;
}

vector<unsigned char> EncryptionThread::cipherBlock(vector<unsigned char> inputBlock, vector<unsigned char> expandedKey)
{
    int round = 0;
    // Add the First round key to the state before starting the rounds.
    AddRoundKey(0, num_words, num_columns, &inputBlock, &expandedKey);
    // There will be num_rounds rounds.
    // The first num_rounds-1 rounds are identical.
    // These num_rounds-1 rounds are executed in the loop below.
    for(round = 1; round < num_rounds; ++round)
    {
      SubBytes(num_words, num_columns, &inputBlock);
      ShiftRows(num_words, num_columns, &inputBlock);
      MixColumns(num_columns, &inputBlock);
      AddRoundKey(round, num_words, num_columns, &inputBlock, &expandedKey);
    }
    // The last round is given below.
    // The MixColumns function is not here in the last round.
    SubBytes(num_words, num_columns, &inputBlock);
    ShiftRows(num_words, num_columns, &inputBlock);
    AddRoundKey(num_rounds, num_words, num_columns, &inputBlock, &expandedKey);
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
void EncryptionThread::AddRoundKey(int round, int rows, int columns, vector<unsigned char>* state, vector<unsigned char>* expandedKey)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      state->at((i*rows) + j) ^= expandedKey->at(round * columns * rows + i * columns + j);
    }
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
void EncryptionThread::SubBytes(int rows, int columns, vector<unsigned char>* state)
{
  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      state->at((i*rows) + j) = getSBoxValue(state->at((i*rows) + j));
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void EncryptionThread::ShiftRows(int rows, int columns, vector<unsigned char>* state)
{
  unsigned char temp;
  for(int rowno = 0; rowno < rows; rowno++)
  {
    for(int shift = 0; shift < rowno; shift++)
    {
        ShiftOnce(rowno * columns, ((rowno + 1) * columns) - 1, state);
    }
  }
}

void EncryptionThread::ShiftOnce(int startIndex, int stopIndex, vector<unsigned char>* state)
{
    unsigned char firstValue = state->at(startIndex);
    for(int idx = startIndex; idx < stopIndex; idx++)
    {
        state->at(idx) = state->at(idx + 1);
    }
    state->at(stopIndex) = firstValue;
}

// MixColumns function mixes the columns of the state matrix
void EncryptionThread::MixColumns(int columns, vector<unsigned char>* state)
{
  unsigned char Tmp,Tm,t;
  for(int colno = 0; colno < num_columns; colno++)
  {
    t   = state->at((0 * columns) + colno);
    Tmp = state->at((0 * columns) + colno) ^ state->at((1 * columns) + colno) ^ state->at((2 * columns) + colno) ^ state->at((3 * columns) + colno);
    Tm  = state->at((0 * columns) + colno) ^ state->at((1 * columns) + colno);
    Tm = xtime(Tm);
    state->at((0 * columns) + colno) ^= Tm ^ Tmp;
    Tm  = state->at((1 * columns) + colno) ^ state->at((2 * columns) + colno);
    Tm = xtime(Tm);
    state->at((1 * columns) + colno) ^= Tm ^ Tmp;
    Tm  = state->at((2 * columns) + colno) ^ state->at((3 * columns) + colno);
    Tm = xtime(Tm);
    state->at((2 * columns) + colno) ^= Tm ^ Tmp;
    Tm  = state->at((3 * columns) + colno) ^ t;
    Tm = xtime(Tm);
    state->at((3 * columns) + colno) ^= Tm ^ Tmp;
  }
}

unsigned char EncryptionThread::xtime(unsigned char x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

void EncryptionThread::padInput(vector<unsigned char>* input)
{
    // If the size is not divisible by 16
    if(input->size() % (num_words * num_columns ) != 0)
    {
        // Keep on adding zeros until the input is divisible by block size
        while(input->size() % (num_words * num_columns ) != 0)
        {
            input->push_back(0x00);
        }
    }
}

vector<unsigned char> EncryptionThread::expandKey(vector<unsigned char> key)
{
    vector<unsigned char> ExpandedKey(key.size()*(num_rounds + 1));
    int round = 0;
    int j = 0;
    int k = 0;
    vector<unsigned char> temp(num_columns); // Used for the column/row operations
    for(round = 0; round < num_words; ++round)
    {
        // The first round key is the key itself.
        ExpandedKey[(round * 4) + 0] = key[(round * 4) + 0];
        ExpandedKey[(round * 4) + 1] = key[(round * 4) + 1];
        ExpandedKey[(round * 4) + 2] = key[(round * 4) + 2];
        ExpandedKey[(round * 4) + 3] = key[(round * 4) + 3];
    }
    // All other round keys are found from the previous round keys.
    for(; (round < (num_columns * (num_rounds + 1))); ++round)
    {
        for(j = 0; j < num_columns; ++j)
        {
            // Copy the previous round's expanded key
            temp[j] = ExpandedKey[(round-1) * 4 + j];
        }
        if (round % num_words == 0)
        {
            // This function rotates the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
            // Rotate Word
            k = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = k;

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.
            // Substitute words
            temp[0] = getSBoxValue(temp[0]);
            temp[1] = getSBoxValue(temp[1]);
            temp[2] = getSBoxValue(temp[2]);
            temp[3] = getSBoxValue(temp[3]);
            temp[0] = temp[0] ^ Rcon[round/num_words];
        }
        else if (num_words > 6 && round % num_words == 4)
        {
            // Substitute words
            temp[0] = getSBoxValue(temp[0]);
            temp[1] = getSBoxValue(temp[1]);
            temp[2] = getSBoxValue(temp[2]);
            temp[3] = getSBoxValue(temp[3]);
        }
        ExpandedKey[round * 4 + 0] = ExpandedKey[(round - num_words) * 4 + 0] ^ temp[0];
        ExpandedKey[round * 4 + 1] = ExpandedKey[(round - num_words) * 4 + 1] ^ temp[1];
        ExpandedKey[round * 4 + 2] = ExpandedKey[(round - num_words) * 4 + 2] ^ temp[2];
        ExpandedKey[round * 4 + 3] = ExpandedKey[(round - num_words) * 4 + 3] ^ temp[3];
    }
}

unsigned char EncryptionThread::getSBoxValue(unsigned char num)
{
  return sbox[num];
}

unsigned char EncryptionThread::getSBoxInvert(unsigned char num)
{
  return rsbox[num];
}

void EncryptionThread::stop()
{
    qDebug() << TAG << " : Stop called from main thread: " << currentThreadId();
    QMutexLocker locker(&m_mutex);
    m_stop = true;
}
