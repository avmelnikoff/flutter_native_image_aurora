import 'package:flutter_native_image_platform_interface/flutter_native_image_platform_interface.dart';

class FlutterNativeImageAurora extends FlutterNativeImageMethodChannel {
  /// Registers this class as the default instance of [FlutterNativeImagePlatformInterface].
  static void registerWith() {
    FlutterNativeImagePlatformInterface.instance = FlutterNativeImageAurora();
  }
}
