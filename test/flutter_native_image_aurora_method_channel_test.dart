// import 'package:flutter/services.dart';
// import 'package:flutter_test/flutter_test.dart';
// import 'package:mp_nfc/mp_nfc_method_channel.dart';

// void main() {
//   TestWidgetsFlutterBinding.ensureInitialized();

//   MethodChannelMpNfc platform = MethodChannelMpNfc();
//   const MethodChannel channel = MethodChannel('mp_nfc');

//   setUp(() {
//     TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(
//       channel,
//       (MethodCall methodCall) async {
//         return '42';
//       },
//     );
//   });

//   tearDown(() {
//     TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger.setMockMethodCallHandler(channel, null);
//   });

//   test('getPlatformVersion', () async {
//     expect(await platform.getPlatformVersion(), '42');
//   });
// }
