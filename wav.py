import wave
import struct

def raw_to_wav(raw_file, wav_file):
    with open(raw_file, "rb") as f:
        raw_audio = f.read()

    with wave.open(wav_file, "wb") as wav:
        n_samples = len(raw_audio) // 2
        wav.setsampwidth(1)
        wav.setnchannels(2)
        wav.setframerate(44100)
        wav.setnframes(n_samples)

        packed_audio = struct.pack(str(n_samples) + "h", *raw_audio)
        wav.writeframes(packed_audio)

# Example usage:
raw_to_wav("dog_bark_right.raw", "output_dog_bark_right.wav")