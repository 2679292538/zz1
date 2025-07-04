QT+=multimedia

HEADERS += \
    $$PWD/WebRtcMoudle/analog_agc.h \
    $$PWD/WebRtcMoudle/complex_fft_tables.h \
    $$PWD/WebRtcMoudle/cpu_features_wrapper.h \
    $$PWD/WebRtcMoudle/defines.h \
    $$PWD/WebRtcMoudle/digital_agc.h \
    $$PWD/WebRtcMoudle/fft4g.h \
    $$PWD/WebRtcMoudle/gain_control.h \
    $$PWD/WebRtcMoudle/noise_suppression.h \
    $$PWD/WebRtcMoudle/noise_suppression_x.h \
    $$PWD/WebRtcMoudle/ns_core.h \
    $$PWD/WebRtcMoudle/nsx_core.h \
    $$PWD/WebRtcMoudle/nsx_defines.h \
    $$PWD/WebRtcMoudle/real_fft.h \
    $$PWD/WebRtcMoudle/resample_by_2_internal.h \
    $$PWD/WebRtcMoudle/ring_buffer.h \
    $$PWD/WebRtcMoudle/signal_processing_library.h \
    $$PWD/WebRtcMoudle/spl_inl.h \
    $$PWD/WebRtcMoudle/typedefs.h \
    $$PWD/WebRtcMoudle/windows_private.h \
    $$PWD/WebRtc_Vad/signal_processing_library.h \
    $$PWD/WebRtc_Vad/spl_inl.h \
    $$PWD/WebRtc_Vad/typedefs.h \
    $$PWD/WebRtc_Vad/vad_core.h \
    $$PWD/WebRtc_Vad/vad_filterbank.h \
    $$PWD/WebRtc_Vad/vad_gmm.h \
    $$PWD/WebRtc_Vad/vad_sp.h \
    $$PWD/WebRtc_Vad/vad_unittest.h \
    $$PWD/WebRtc_Vad/webrtc_vad.h \
    $$PWD/audioread.h \
    $$PWD/audiowrite.h \
    $$PWD/world.h

SOURCES += \
    $$PWD/WebRtcMoudle/analog_agc.c \
    $$PWD/WebRtcMoudle/complex_bit_reverse.c \
    $$PWD/WebRtcMoudle/complex_fft.c \
    $$PWD/WebRtcMoudle/copy_set_operations.c \
    $$PWD/WebRtcMoudle/cross_correlation.c \
    $$PWD/WebRtcMoudle/digital_agc.c \
    $$PWD/WebRtcMoudle/division_operations.c \
    $$PWD/WebRtcMoudle/dot_product_with_scale.c \
    $$PWD/WebRtcMoudle/downsample_fast.c \
    $$PWD/WebRtcMoudle/energy.c \
    $$PWD/WebRtcMoudle/fft4g.c \
    $$PWD/WebRtcMoudle/get_scaling_square.c \
    $$PWD/WebRtcMoudle/min_max_operations.c \
    $$PWD/WebRtcMoudle/noise_suppression.c \
    $$PWD/WebRtcMoudle/noise_suppression_x.c \
    $$PWD/WebRtcMoudle/ns_core.c \
    $$PWD/WebRtcMoudle/nsx_core.c \
    $$PWD/WebRtcMoudle/nsx_core_c.c \
    $$PWD/WebRtcMoudle/nsx_core_neon_offsets.c \
    $$PWD/WebRtcMoudle/real_fft.c \
    $$PWD/WebRtcMoudle/resample.c \
    $$PWD/WebRtcMoudle/resample_48khz.c \
    $$PWD/WebRtcMoudle/resample_by_2.c \
    $$PWD/WebRtcMoudle/resample_by_2_internal.c \
    $$PWD/WebRtcMoudle/resample_by_2_mips.c \
    $$PWD/WebRtcMoudle/resample_fractional.c \
    $$PWD/WebRtcMoudle/ring_buffer.c \
    $$PWD/WebRtcMoudle/spl_init.c \
    $$PWD/WebRtcMoudle/spl_sqrt.c \
    $$PWD/WebRtcMoudle/spl_sqrt_floor.c \
    $$PWD/WebRtcMoudle/splitting_filter.c \
    $$PWD/WebRtcMoudle/vector_scaling_operations.c \
    $$PWD/WebRtc_Vad/vad_core.c \
    $$PWD/WebRtc_Vad/vad_filterbank.c \
    $$PWD/WebRtc_Vad/vad_gmm.c \
    $$PWD/WebRtc_Vad/vad_sp.c \
    $$PWD/WebRtc_Vad/webrtc_vad.c \
    $$PWD/audioread.cpp \
    $$PWD/audiowrite.cpp


LIBS += $$PWD/speex/lib/libspeex.lib\
        $$PWD/WebRtc_Vad/signalProcess.lib
