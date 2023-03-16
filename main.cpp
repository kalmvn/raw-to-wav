#include <iostream>
#include "AudioFile.h"
#include <fstream>
#include <stdio.h>
#include <string.h>

#define AUD_DATA_OFFSET 44
#define BUFSIZE 512

#define N_SAMPLES 882000 // Number of samples in the raw audio file
#define SAMPLE_RATE 44100 // Sample rate of the raw audio file
#define NUM_CHANNELS 2 // Number of channels (mono)
#define BITS_PER_SAMPLE 16 // Bits per sample in the raw audio file
#define NUM_SECONDS 1
#define AUD_LEN_LIMIT32 (NUM_SECONDS * SAMPLE_RATE * (32 / 8))
#define AUD_LEN_LIMIT (NUM_SECONDS * SAMPLE_RATE * (32 / 8))

using namespace std;
AudioFile<float> audioFile;
void initWaveFile(int raw_size, FILE *output);
// Struct containing the Wave Audio File Format
struct WAVHeader {
    char chunkId[4];        // "RIFF"
    unsigned int chunkSize; // Total file size - 8
    char format[4];         // "WAVE"
    char subchunk1Id[4];    // "fmt "
    unsigned int subchunk1Size; // 16 for PCM
    unsigned short audioFormat; // 1 for PCM, 3 for IEEE Float
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    char subchunk2Id[4];    // "data"
    unsigned int subchunk2Size; // size of the audio data
};

int main()
{
    /*
    le_fs_FileRef_t fp;
	le_fs_Open("/aud_rec_try.raw", LE_FS_RDWR | LE_FS_CREAT,&fp);

	int dummy = 0;
	le_fs_Seek(fp,0,LE_FS_SEEK_END,&dummy);
	le_fs_Write(fp,RawAudioRecord,TotalLen);

	le_fs_Close(fp);*/

//    char *inputFileName = "aud_rec_try.raw";
//    char *inputFileName = "dog_bark_right.raw";
    char *inputFileName = "aud_rec_try_R1.raw";

    FILE *inputFile = fopen(inputFileName, "rb");

    int TotalLen = 0;
    fseek(inputFile, 0L, SEEK_END);
    TotalLen = ftell(inputFile);
    rewind(inputFile);
    printf("Total Len: %d \n", TotalLen);

    if (!inputFile)
    {
        printf("Failed to open input file: %s\n", inputFileName);
    }

    //char *inputFileName = "/data/le_fs/aud_rec_try.raw";
    char *outputFileName = "aud_rec_try_R1_win.wav";
    //char *outputFileName = "aud_rec_try_left_test5.wav";

    //Write to wav
    FILE *outputFile = fopen(outputFileName, "wb");
    if (!outputFile)
    {
        printf("Failed to open output file: %s\n", outputFileName);
        fclose(outputFile);
    }



    //Save wav file
    //fseek(outputFile, 0L, AUD_DATA_OFFSET);

    if (!outputFile)
    {
        printf("Failed to open output file: %s\n", outputFileName);
        fclose(outputFile);
    }

    static uint16_t aud_ctr = 0;
    //uint16_t output_sample = 0;
    size_t samples_read = 0;
    //int aud_len_limit = AUD_LEN_LIMIT;
    uint8_t input_sample[AUD_LEN_LIMIT]= {0};


    int seconds = 10;



    /*
    while ((samples_read = fread(input_sample, sizeof(uint8_t), AUD_LEN_LIMIT/*sizeof(buffer), inputFile)) > 0)
    {
        for(size_t i = 0 ; i < samples_read ; i++ )
        {
            output_sample[i] = (uint16_t)(input_sample[i] * 256);
        }
        //std::cout << "Samples read :" << samples_read << endl;
        fwrite(output_sample, 2, samples_read, outputFile);
    }
    */
    int aud_len_limit = 10 * AUD_LEN_LIMIT;

    //Example of reading a chunk
    /*
    size_t samplesRead = fread(input_sample, 1, aud_len_limit, inputFile);
    std::cout << "samplesRead: " << samplesRead << endl;
    for(uint32_t i = 0 ; i < samplesRead ; i++)
    {
        output_sample[i] = uint16_t ( input_sample[i] * 256 );

    }

    size_t bytesWritten = fwrite(output_sample, 1, aud_len_limit, outputFile);*/

    // Calculate the number of bytes to read

    size_t n;
    size_t bytes_to_read = SAMPLE_RATE * NUM_CHANNELS * seconds;
    cout << "bytes_to_read: " << bytes_to_read << endl;
    int chunk_size;
    size_t bytesWritten;

    /*
    int8_t buffer8[BUFSIZE];
    int16_t buffer16[BUFSIZE];
    float buffer32[BUFSIZE];*/
        // Read the file in chunks
        /*
    while (bytes_to_read > 0) {
        size_t chunk_size = (bytes_to_read > BUFSIZE * sizeof(int16_t)) ? BUFSIZE * sizeof(int16_t) : bytes_to_read;
        n = fread(buffer8, 1, chunk_size, inputFile);

        // Process the audio data
        // ...
        // Convert the 8-bit samples to 16-bit samples
        for (int i = 0; i < n; i++) {
            buffer32[i] =   (float) buffer8[i] / 255.0f;
        }
        bytesWritten = fwrite(buffer32, sizeof(float), chunk_size, outputFile);

        bytes_to_read -= n;
    }*/
    uint32_t num_samples = SAMPLE_RATE * NUM_CHANNELS * seconds;
    cout << "num_samples" << num_samples << endl;
    uint32_t samples_written = 0;

    initWaveFile(num_samples, outputFile);

    int8_t *buffer8 = (int8_t*)malloc(sizeof(int8_t) * num_samples);
    //int16_t *buffer16 = (int16_t*)malloc(sizeof(int16_t) * num_samples);
    float *buffer32 = (float*)malloc(sizeof(float) * num_samples);

    while (samples_written < num_samples) {
        n = fread(buffer8, 1, num_samples, inputFile);
        for (size_t i = 0; i < n; i++) {
            buffer32[i] = (float)buffer8[i] / 128.0f;
            //buffer16[i] = (int16_t)buffer8[i] * 256;// (int16_t) buffer8[i] * 256;//(float)buffer8[i] / 255.0f; // convert 8-bit sample to floating-point
        }
        //fwrite(buffer16, 2, n, outputFile); // write the floating-point samples to the output file
        fwrite(buffer32, sizeof(float), n, outputFile);
        samples_written += n;
    }

    std::cout << "samples_written" << samples_written << endl;


    rewind(outputFile);
    fseek(outputFile, 0L, SEEK_END);
    int TotalLen2 = ftell(outputFile);
    printf("The wav file total length is %d \n", TotalLen2);

    free(buffer8);
    free(buffer32);
    fclose(outputFile);
    fclose(inputFile);


    std::string filePath = "aud_rec_try_R1.wav";
    std::cout << "================== Checking File:  " << filePath << "================"  <<  endl;
    bool loadedOK = audioFile.load (filePath);
    assert (loadedOK);
    std::cout << "Bit Depth: " << audioFile.getBitDepth() << std::endl;
    std::cout << "Sample Rate: " << audioFile.getSampleRate() << std::endl;
    std::cout << "Num Channels: " << audioFile.getNumChannels() << std::endl;
    std::cout << "Length in Seconds: " << audioFile.getLengthInSeconds() << std::endl;
    std::cout << "Num Samples Per Channel: " << audioFile.getNumSamplesPerChannel() << std::endl;
    std::cout << std::endl;

    std::cout << "==================Sample Check ================" << endl;
    std::vector <float> test_vect;
    for(int i = 0 ; i < audioFile.getNumSamplesPerChannel() ; i++)
    {
        //audioFile.samples[0][i];0.078125, 0.101562, 0.0859375
        //, 0.09375, 0.0859375, 0.078125, 0.0625, 0.0546875, 0.0625
        //, 0.0390625, 0.046875, 0.0234375, 0.03125, 0.03125, 0.0156
        //std::cout << audioFile.samples[0][i] << ", ";
    }



    std::cout << "==================Sample Check END ================" << endl;

    std::cout << "================== FORMAT WAV FILE ================" << endl;


    return 0;
}

void initWaveFile(int raw_size, FILE *output)//struct WAVHeader wav_header)
{

    uint32_t wav_size = 36 + (raw_size * (BITS_PER_SAMPLE / 8));

    // Write the WAV header to the output file
    fwrite("RIFF", 4, 1, output); // ChunkID
    fwrite(&wav_size, 4, 1, output); // ChunkSize
    fwrite("WAVE", 4, 1, output); // Format
    fwrite("fmt ", 4, 1, output); // Subchunk1ID
    uint32_t subchunk1_size = 16; //16-Bit PCM
    fwrite(&subchunk1_size, 4, 1, output); // Subchunk1Size
    uint16_t audio_format = 1; //Audio Format 1 - PCM
    fwrite(&audio_format, 2, 1, output); // AudioFormat
    uint16_t num_channels = NUM_CHANNELS;
    fwrite(&num_channels, 2, 1, output); // NumChannels
    uint32_t sample_rate = SAMPLE_RATE;
    fwrite(&sample_rate, 4, 1, output); // SampleRate
    uint32_t byte_rate = SAMPLE_RATE * NUM_CHANNELS * (BITS_PER_SAMPLE / 8);
    fwrite(&byte_rate, 4, 1, output); // ByteRate
    uint16_t block_align = NUM_CHANNELS * (BITS_PER_SAMPLE / 8);
    fwrite(&block_align, 2, 1, output); // BlockAlign
    uint16_t bits_per_sample = BITS_PER_SAMPLE;
    fwrite(&bits_per_sample, 2, 1, output); // BitsPerSample
    fwrite("data", 4, 1, output); // Subchunk2ID
    uint32_t subchunk2_size = raw_size * (BITS_PER_SAMPLE / 8);
    fwrite(&subchunk2_size, 4, 1, output); // Subchunk2Size
}
