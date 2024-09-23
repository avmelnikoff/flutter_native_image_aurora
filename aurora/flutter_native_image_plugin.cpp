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

	std::filesystem::path origPath(fileArgument);
	std::filesystem::path tempPath(std::tmpnam(nullptr));

	auto extension = origPath.extension().string();
	if (!checkExtension(extension)) {
		extension = ".jpg";
	}

	tempPath.replace_extension(std::filesystem::path {extension});

	QImage origImage(QString::fromStdString(fileArgument));

	auto newWidth = (widthArgument == 0 ? (origImage.width() / 100 * percentageArgument) : widthArgument);
  auto newHeight = (heightArgument == 0 ? (origImage.height() / 100 * percentageArgument) : heightArgument);

	QImage scaledImage = origImage.scaled(newWidth, newHeight, Qt::KeepAspectRatio);

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

	std::filesystem::path origPath(fileArgument);
	std::filesystem::path tempPath(std::tmpnam(nullptr));

	auto extension = origPath.extension().string();
	if (!checkExtension(extension)) {
		extension = ".jpg";
	}

	tempPath.replace_extension(std::filesystem::path {extension});

	QImage origImage(QString::fromStdString(fileArgument));

	if (originXArgument < 0 
		|| originYArgument < 0 
		|| originXArgument > origImage.width() 
		|| originYArgument > origImage.height()
		|| originXArgument+widthArgument > origImage.width() 
		|| originYArgument+heightArgument > origImage.height()) {
		result->Error("bounds_error", "Bounds are outside of the dimensions of the source image");
		return;
	}

	QImage scaledImage = origImage.copy(originXArgument, originYArgument, widthArgument, heightArgument);

	auto isSaved = scaledImage.save(QString::fromStdString(tempPath.string()));
	if (isSaved) {
		result->Success(tempPath.string());
	}
	else {
		result->Error("create_error", "Temporary file could not be created");
	}

	return;
}

bool FlutterNativeImagePlugin::compare(const std::string& str1, const std::string& str2) {
	if (str1.length() != str2.length())
		return false;

	for (int i = 0; i < str1.length(); ++i) {
		if (std::tolower(str1[i]) != std::tolower(str2[i]))
			return false;
	}

	return true;
}

bool FlutterNativeImagePlugin::checkExtension(const std::string& ext) {
	const std::string extPng = ".png";
	const std::string extJpg = ".jpg";
	const std::string extJpeg = ".jpeg";
	return !(ext.empty() 
			|| !compare(ext, extPng) 
			|| !compare(ext, extJpg) 
			|| !compare(ext, extJpeg));
}
