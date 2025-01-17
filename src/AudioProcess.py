from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume, IAudioEndpointVolume, IAudioMeterInformation
import logging
from ctypes import POINTER, cast
from comtypes import CLSCTX_ALL

# logging.basicConfig(
#     level=logging.ERROR,
#     format="%(asctime)s [%(levelname)s] %(message)s",
#     handlers=[
#         logging.StreamHandler()
#     ]
# )
# logger = logging.getLogger()

class AudioProcess:
    def __init__(self):
        self.sessions = {}
        self.interface = None
        self.fetch_all_audio_sessions()
        self.fetch_speakers()

    def fetch_all_audio_sessions(self):
        sessions = AudioUtilities.GetAllSessions()
        for session in sessions:
            if session.Process:
                volume = session._ctl.QueryInterface(ISimpleAudioVolume)
                # devices = AudioUtilities.GetSpeakers()
                # interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
                # volume = cast(interface, POINTER(IAudioEndpointVolume))
                self.sessions[session.Process.name()] = volume
        # logger.info("--AudioProcess--get_all_audio_sessions-- Sessions fetched")

    def fetch_speakers(self):
        devices = AudioUtilities.GetSpeakers()
        self.interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)

        # logger.info("--AudioProcess--get_speakers-- Interface fetched")

    def get_speaker_volume(self):
        peak = self.interface.QueryInterface(IAudioMeterInformation).GetPeakValue()

        return peak
    
    def mute_speaker(self):
        self.interface.QueryInterface(IAudioEndpointVolume).SetMute(1, None)

        # logger.info(f"--AudioProcess--mute_speaker Interface muted")
    
    def unmute_speaker(self):
        self.interface.QueryInterface(IAudioEndpointVolume).SetMute(0, None)

        # logger.info(f"--AudioProcess--unmute_speaker Interface unmuted")

    def set_volume(self, process_name, set_to):
        volume = self.sessions[process_name]
        volume.SetMasterVolume(set_to, None)

        # logger.info(f"--AudioProcess--set_volume Process {process_name} volume set to: {set_to}")

    def get_volume_level(self, process_name):
        volume = self.sessions[process_name]
        
        return volume

    def mute(self, process_name, mute):
        volume = self.sessions[process_name]
        volume.SetMute(mute, None)

        # logger.info(f"--AudioProcess--mute Process {process_name} muted: {mute}")