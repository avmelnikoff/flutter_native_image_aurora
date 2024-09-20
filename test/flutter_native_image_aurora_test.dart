// import 'package:flutter_test/flutter_test.dart';
// import 'package:mp_nfc/mp_nfc.dart';
// import 'package:mp_nfc/mp_nfc_platform_interface.dart';
// import 'package:mp_nfc/mp_nfc_method_channel.dart';
// import 'package:plugin_platform_interface/plugin_platform_interface.dart';

// class MockMpNfcPlatform
//     with MockPlatformInterfaceMixin
//     implements MpNfcPlatform {

//   @override
//   Future<String?> getPlatformVersion() => Future.value('42');
// }

// void main() {
//   final MpNfcPlatform initialPlatform = MpNfcPlatform.instance;

//   test('$MethodChannelMpNfc is the default instance', () {
//     expect(initialPlatform, isInstanceOf<MethodChannelMpNfc>());
//   });

//   test('getPlatformVersion', () async {
//     MpNfc mpNfcPlugin = MpNfc();
//     MockMpNfcPlatform fakePlatform = MockMpNfcPlatform();
//     MpNfcPlatform.instance = fakePlatform;

//     expect(await mpNfcPlugin.getPlatformVersion(), '42');
//   });
// }
