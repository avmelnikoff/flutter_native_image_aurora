#include <filesystem>
#include <iostream>
#include <cstdio>
#include <string>

#include <QString>
#include <QImage>

#include <flutter_native_image_aurora/globals.h>
#include <flutter_native_image_aurora/encodable_helper.h>
#include <flutter_native_image_aurora/flutter_native_image_plugin.h>

namespace Channels {
constexpr auto Methods = "flutter_native_image";
} // namespace Channels

namespace Methods {
constexpr auto GetImageProperties = "getImageProperties";
constexpr auto CompressImage = "compressImage";
constexpr auto CropImage = "cropImage";
} // namespace Methods

void FlutterNativeImagePlugin::RegisterWithRegistrar(
	flutter::PluginRegistrar *registrar
	) {
	// Create MethodChannel with StandardMethodCodec
	auto methodChannel = std::make_unique<MethodChannel>(
		registrar->messenger(), 
		Channels::Methods,
		&flutter::StandardMethodCodec::GetInstance()
	);

	// Create plugin
	std::unique_ptr<FlutterNativeImagePlugin> plugin(
		new FlutterNativeImagePlugin(std::move(methodChannel))
	);

	// Register plugin
	registrar->AddPlugin(std::move(plugin));
}

FlutterNativeImagePlugin::FlutterNativeImagePlugin(
	std::unique_ptr<MethodChannel> methodChannel
	) : m_methodChannel(std::move(methodChannel)) {
	// Create MethodHandler
	RegisterMethodHandler();
}

void FlutterNativeImagePlugin::RegisterMethodHandler() {
	m_methodChannel->SetMethodCallHandler(
		[this](const MethodCall &call, std::unique_ptr<MethodResult> result) {
			if (call.method_name().compare(Methods::GetImageProperties) == 0) {
				onGetImageProperties(call, std::move(result));
			} else if (call.method_name().compare(Methods::CompressImage) == 0) {
				onCompressImage(call, std::move(result));
			} else if (call.method_name().compare(Methods::CropImage) == 0) {
				onCropImage(call, std::move(result));
			} else {
				result->NotImplemented();
			}
		}
	);
}

void FlutterNativeImagePlugin::onGetImageProperties(
	const MethodCall &call, std::unique_ptr<MethodResult> result
	) {
	auto arguments = EncodableHelper::GetValue<EncodableMap>(*call.arguments());
	auto fileArgument = EncodableHelper::GetString(arguments, "file");

	std::cout << "FlutterNativeImagePlugin::onGetImageProperties: fileArgument: " << fileArgument << std::endl;

	QImage image(QString::fromStdString(fileArgument));

	result->Success(EncodableValue(EncodableMap{
		{EncodableValue("width"), EncodableValue(image.width())},
		{EncodableValue("height"), EncodableValue(image.height())},
		{EncodableValue("orientation"), EncodableValue(0)},
	}));

	return;
}

void FlutterNativeImagePlugin::onCompressImage(
	const MethodCall &call, std::unique_ptr<MethodResult> result
	) {
	auto arguments = EncodableHelper::GetValue<EncodableMap>(*call.arguments());
	auto fileArgument = EncodableHelper::GetString(arguments, "file");
	auto qualityArgument = EncodableHelper::GetInt(arguments, "quality");
	auto percentageArgument = EncodableHelper::GetInt(arguments, "percentage");
	auto widthArgument = EncodableHelper::GetInt(arguments, "targetWidth");
	auto heightArgument = EncodableHelper::GetInt(arguments, "targetHeight");

	std::cout << "FlutterNativeImagePlugin::onCompressImage: fileArgument: " << fileArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: qualityArgument: " << qualityArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: percentageArgument: " << percentageArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: widthArgument: " << widthArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: heightArgument: " << heightArgument << std::endl;

	std::filesystem::path origPath(fileArgument);
	std::filesystem::path tempPath(std::tmpnam(nullptr));

	std::cout << "FlutterNativeImagePlugin::onCompressImage: tempPath extension: " << tempPath.extension() << std::endl;

	if (!origPath.extension().empty()) {
		tempPath.replace_extension(origPath.extension());
	}

	std::cout << "FlutterNativeImagePlugin::onCompressImage: tempPath: " << tempPath << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: tempPath extension: " << tempPath.extension() << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: filename: " << origPath.filename() << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: stem: " << origPath.stem() << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: extension: " << origPath.extension() << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: parent_path: " << origPath.parent_path() << std::endl;

	QImage origImage(QString::fromStdString(fileArgument));

	auto newWidth = (widthArgument == 0 ? (origImage.width() / 100 * percentageArgument) : widthArgument);
  auto newHeight = (heightArgument == 0 ? (origImage.height() / 100 * percentageArgument) : heightArgument);

	std::cout << "FlutterNativeImagePlugin::onCompressImage: width: " << origImage.width() << " newWidth: " << newWidth << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: height: " << origImage.height() << " newHeight: " << newHeight << std::endl;

	QImage scaledImage = origImage.scaled(newWidth, newHeight, Qt::KeepAspectRatio);

	std::cout << "FlutterNativeImagePlugin::onCompressImage: width: " << scaledImage.width() << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCompressImage: height: " << scaledImage.height() << std::endl;

	auto isSaved = scaledImage.save(QString::fromStdString(tempPath.string()));
	if (isSaved) {
		result->Success(tempPath.string());
	}
	else {
		result->Error("create_error", "Temporary file could not be created");
	}

	return;
}

void FlutterNativeImagePlugin::onCropImage(
	const MethodCall &call, std::unique_ptr<MethodResult> result
	) {
	auto arguments = EncodableHelper::GetValue<EncodableMap>(*call.arguments());
	auto fileArgument = EncodableHelper::GetString(arguments, "file");
	auto originXArgument = EncodableHelper::GetInt(arguments, "originX");
	auto originYArgument = EncodableHelper::GetInt(arguments, "originY");
	auto widthArgument = EncodableHelper::GetInt(arguments, "width");
	auto heightArgument = EncodableHelper::GetInt(arguments, "height");

	std::cout << "FlutterNativeImagePlugin::onCropImage: fileArgument: " << fileArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCropImage: originXArgument: " << originXArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCropImage: originYArgument: " << originYArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCropImage: widthArgument: " << widthArgument << std::endl;
	std::cout << "FlutterNativeImagePlugin::onCropImage: heightArgument: " << heightArgument << std::endl;

	// TODO: implement the method!

	// result->Success(finalFileName);
	result->Error("create_error", "Temporary file could not be created");

	return;
}
