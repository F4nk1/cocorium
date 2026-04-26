package tracer

import (
	"context"
	"log"
	"sync"

	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
)

type contextkey string

const RequestIDKey contextkey = "requestID"
const UserIdKey contextkey = "userID"

var (
	logger *zap.SugaredLogger
	once   sync.Once
)

func Errorf(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).Errorf(format, args...)
}

func Infof(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).Infof(format, args...)
}

func Debugf(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).Debugf(format, args...)
}

func Warnf(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).Warnf(format, args...)
}

func Fatalf(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).Fatalf(format, args...)
}

func Panicf(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).Panicf(format, args...)
}

func DPanicf(ctx context.Context, format string, args ...interface{}) {
	WithContext(ctx).DPanicf(format, args...)
}

func Error(ctx context.Context, args ...interface{}) {
	WithContext(ctx).Error(args...)
}

func Info(ctx context.Context, args ...interface{}) {
	WithContext(ctx).Info(args...)
}

func Debug(ctx context.Context, args ...interface{}) {
	WithContext(ctx).Debug(args...)
}

func Warn(ctx context.Context, args ...interface{}) {
	WithContext(ctx).Warn(args...)
}

func Fatal(ctx context.Context, args ...interface{}) {
	WithContext(ctx).Fatal(args...)
}

func Panic(ctx context.Context, args ...interface{}) {
	WithContext(ctx).Panic(args...)
}

func DPanic(ctx context.Context, args ...interface{}) {
	WithContext(ctx).DPanic(args...)
}

func WithContext(ctx context.Context) *zap.SugaredLogger {
	once.Do(func() {
		logger = initLogger()
	})
	l:= logger

	if RequestIDKey, ok := ctx.Value(RequestIDKey).(string); ok {
		l = l.With("requestID", RequestIDKey)
	}

	if UserIdKey, ok := ctx.Value(UserIdKey).(string); ok {
		l = l.With("userID", UserIdKey)
	}

	return l
}

func initLogger() *zap.SugaredLogger {
	config := zap.Config{
		Level: 	 zap.NewAtomicLevelAt(zap.DebugLevel),
		Development: false,
		Encoding:   "console",
		EncoderConfig: zapcore.EncoderConfig{
			MessageKey: 	"message",
			LevelKey:   	"level",
			TimeKey:    	"time",
			NameKey:    	"name",
			CallerKey:  	"caller",
			StacktraceKey: 	"stack",
			LineEnding: 	"\n",
			EncodeLevel: 	zapcore.CapitalColorLevelEncoder,
			EncodeTime: 	zapcore.ISO8601TimeEncoder,
			EncodeDuration: zapcore.StringDurationEncoder,
			EncodeCaller:  zapcore.ShortCallerEncoder,
		},
		OutputPaths:      []string{"stderr"},
		ErrorOutputPaths: []string{"stderr"},
	}
	zapLogger, err := config.Build()
	if err != nil {
		log.Fatalf("Failed to initialize logger: %v", err)
	}
	return zapLogger.Sugar()		
}