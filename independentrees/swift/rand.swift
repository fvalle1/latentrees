import Foundation
import Metal

enum FileWriteError: Error {
case directoryDoesntExist
case convertToDataIssue
}

let srcString = """
        #include <metal_stdlib>
        using namespace metal;

        kernel void storyMersenne(
            const device uint *vIn [[ buffer(0) ]],
            device int *vOut [[ buffer(1) ]],
            uint id[[ thread_position_in_grid ]]){
          
          mt19937 mers;

          mers.srand(vIn[id]);
         
          float temp = 10.0;

          for (uint l = 0; l < 100; l++){
              float delta = 1.0 + metal::sqrt(3.0)*metal::abs(temp);
              float r = mers.rand();
              temp = metal::round(2*r*delta+temp-delta);
              if(temp > 1000000000000000){
              temp = 1000000000000000;
              }
          }

          vOut[id] = metal::round(temp);
        }
""";

let devices = MTLCopyAllDevices();

print("Found \(devices.count) devices")

for device in devices {
    print(device.name as Any)
}

let device = devices[0];

let dataSize = 1024;
let statistics = 10;

let compileOptions = MTLCompileOptions();
let library_rand = try! String(contentsOfFile: "/Volumes/LaCie/phd/latentrees/independentrees/swift/random.metal");
let library_mersenne = try! String(contentsOfFile: "/Volumes/LaCie/phd/latentrees/independentrees/swift/mersenne.metal");

let source = library_rand+library_mersenne+srcString;

let library = try! device.makeLibrary(source: source, options: compileOptions);
let runFunction = library.makeFunction(name: "storyMersenne") ?? nil;

let computePipelineState = try? device.makeComputePipelineState(function: runFunction!);

// let inDataPtr = inBuffer?.contents().bindMemory(to: Int32.self, capacity: dataSize * MemoryLayout<Int32>.size);
// let inDataBuffer = UnsafeBufferPointer(start: inDataPtr, count: dataSize)
// var inData = Array(inDataBuffer);

var inData:[Int32] = Array(repeating: 1, count: dataSize);

// let inBuffer = device.makeBuffer(length: dataSize * MemoryLayout<Int32>.size, options: MTLResourceOptions.storageModeManaged);
let outBuffer = device.makeBuffer(length: dataSize * MemoryLayout<Int32>.size, options: MTLResourceOptions.storageModeManaged);


guard let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first else {
    throw FileWriteError.directoryDoesntExist
}

let fileName = "randoms_swift.dat"
let fileUrl = dir.appendingPathComponent(fileName)

let start = DispatchTime.now().uptimeNanoseconds // <<<<<<<<<< Start time
for istat in 0..<statistics{

    for i in 0..<dataSize{
        inData[i] = Int32(istat*dataSize+i);
    }
    let inBuffer = device.makeBuffer(bytes: inData, length: dataSize * MemoryLayout<Int32>.size, options: MTLResourceOptions.storageModeManaged);

    inBuffer?.didModifyRange(Range<Int>(NSMakeRange(0,dataSize * MemoryLayout<Int32>.size))!);

    let commandQueue = device.makeCommandQueue();

    let commandBuffer = commandQueue?.makeCommandBuffer();
    let commandEncoder = commandBuffer?.makeComputeCommandEncoder();
    commandEncoder?.setComputePipelineState(computePipelineState!);
    commandEncoder?.setBuffer(inBuffer, offset: 0, index:0);
    commandEncoder?.setBuffer(outBuffer, offset: 0, index:1);
    commandEncoder?.dispatchThreadgroups(
        MTLSize(width:1,height:1,depth:1),
        threadsPerThreadgroup: MTLSize(width:dataSize,height:1,depth:1)
    );
    commandEncoder?.endEncoding();


    let blitCommandEncoder = commandBuffer?.makeBlitCommandEncoder();
    blitCommandEncoder?.synchronize(resource: outBuffer!);
    blitCommandEncoder?.endEncoding();

    commandBuffer?.commit();
    commandBuffer?.waitUntilCompleted();


//     let outDataPtr = outBuffer?.contents().bindMemory(to: Int32.self, capacity: dataSize * MemoryLayout<Int32>.size);
//     let outDataBuffer = UnsafeBufferPointer(start: outDataPtr, count: dataSize)
//     let outData = Array(outDataBuffer);


//   if let fileHandle = FileHandle(forWritingAtPath: fileUrl.path) {
//       fileHandle.seekToEndOfFile()
//     for out in outData{
//         guard let data = (String(out)+"\n").data(using: String.Encoding.utf8) else {
//             throw FileWriteError.convertToDataIssue
//         }
//         fileHandle.write(data)
//     }
//   }else{
//     for out in outData{
//         let text = String(out)+"\n";
//         try text.write(to: fileUrl, atomically: false, encoding: String.Encoding.utf8)
//     }
//   }

}


let end = DispatchTime.now().uptimeNanoseconds   // <<<<<<<<<<   end time
print("Time difference (GPU[Swift]) = \((end-start)/1000000) [ms]");
